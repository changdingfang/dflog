//***************************************************************
// @file:    normalSink.cpp
// @author:  dingfang
// @date    2020-06-12 18:45:46
//***************************************************************

#include <dflog/normalSink.h>
#include <dflog/common.h>

#include <time.h>
#include <sys/time.h>

#include <vector>


namespace dflog
{

	namespace sinks
	{

		NormalSink::NormalSink(std::string filename, Rotation_T rotationTime, uint64_t maxFilesize, uint16_t maxFiles) 
			/* default maxFilesize = 1024 * 1024 * 1024; maxFiles = 10 */
			: filename_(std::move(filename))
			, rotation_(std::move(rotationTime))
			, currentSize_(0)
			, maxFilesize_(maxFilesize)
			, maxFiles_(maxFiles)
		{
			struct timeval tv;
			::gettimeofday(&tv, nullptr);
			struct tm t;
			::localtime_r(&tv.tv_sec, &t);
			t.tm_hour	= rotation_.rotationH;
			t.tm_min	= rotation_.rotationM;
			t.tm_sec	= 0;
			rotationTime_.sec = std::mktime(&t);
			if (rotationTime_.sec <= tv.tv_sec)
			{
				rotationTime_.sec += ONE_DAY_SEC;
			}
			rotationTime_.usec = 0;

			fileHelper_.open(filename_);
			currentSize_ = fileHelper_.size();
		}

		void NormalSink::log(const dflog::LogMsg_T &msg)
		{
			std::lock_guard<dflog::Mutex> lock(Sink::mutex_);

			formatBuf_t formattedMsg;
			/* format */
			Sink::formatHelper_->format(msg, formattedMsg);

			currentSize_ += formattedMsg.size();
			dflog::LogClock_T time = msg.time;
			bool shouldRotate = (msg.time > rotationTime_) || (currentSize_ > maxFilesize_);

			if (shouldRotate)
			{
				this->rotateLog_(time.sec);
				do
				{
					if (msg.time > rotationTime_)
					{
						rotationTime_ .sec += ONE_DAY_SEC;
					}
				} while (msg.time > rotationTime_);

				currentSize_ = formattedMsg.size();
			}

			/* I/O */
			fileHelper_.write(formattedMsg);

			if (Sink::shouldFlush(msg.level))
			{
				this->flush();
			}

			return ;
		}


		void NormalSink::flush()
		{
			fileHelper_.flush();
		}


		void NormalSink::setPattern(const std::string &pattern)
		{
			return ;
		}

		void NormalSink::setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper)
		{
			Sink::formatHelper_ = std::move(sinkFormatHelper);
			return ;
		}


		void NormalSink::rotateLog_(time_t time)
		{
			std::string::size_type pos = filename_.find_last_of('.');
			std::string ext = (pos != std::string::npos ? filename_.substr(pos + 1, filename_.size()) : filename_);

			std::string backname(pos != std::string::npos ? filename_.substr(0, pos) : filename_);
			struct tm t;
			::localtime_r(&time, &t);
			/* 以时间分割, 当文件名相同时, 序号分割. */
			/* 1. filename__year-mon-day.log */
			/*    or */
			/* 2. filename__year-mon-day.log.1 */
			char s[64] = { 0 };
			strftime(s, sizeof(s), DFLOG_LOG_BACKUP_FORMAT, &t);
			backname.append(s);
			if (!ext.empty())
			{
				backname.append(".");
				backname.append(ext);
			}

			if (::access(backname.c_str(), F_OK) == 0)
			{
				int num = 0;
				std::vector<std::string> filenamesStk;
				do
				{
					std::string name(backname);
					name.append(".");
					name.append(std::to_string(++num));
					filenamesStk.push_back(name);
					if (::access(name.c_str(), F_OK) != 0)
					{
						break;
					}
				} while (true);

				do
				{
					std::string filename2(std::move(filenamesStk.back()));
					filenamesStk.pop_back();
					if (filenamesStk.size() <= 0)
					{
						dflog::os::rename(backname, filename2);
						break;
					}
					std::string filename1(filenamesStk.back());
					dflog::os::rename(filename1, filename2);
				} while (filenamesStk.size() > 0);
			}

			fileHelper_.close();
			dflog::os::rename(filename_, backname);
			fileHelper_.open(filename_);
			return ;
		}

	}; /* namespace sinks end */
}; /* namespace dflog end */
