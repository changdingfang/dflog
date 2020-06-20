//***************************************************************
// @file:    formatHelper.cpp
// @author:  dingfang
// @date    2020-06-13 10:48:53
//***************************************************************



#include <dflog/formatHelper.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include <cstdio>
#include <ctime>

namespace dflog
{
	namespace fmtHelper
	{

		FormatHelper::FormatHelper()
		{
			logBuf_ = new char[maxLogBufLen_ + 1]();
		}
		FormatHelper::~FormatHelper()
		{
			if (logBuf_ != nullptr)
			{
				delete logBuf_;
				logBuf_ = nullptr;
			}
		}

		void FormatHelper::format(const LogMsg_T &logMsg, formatBuf_t &buf)
		{
			if (lastSec_ != logMsg.time.sec)
			{
				this->updateTime_(logMsg.time.sec);
			}
			buf = std::move(fmt::format("[{}.{:03}] [{}] [{}] [{}]({}): {}\n",
					lastTime_,
					logMsg.time.usec / 1000,
					dflog::level::LEVEL[logMsg.level],
					/* logMsg.logName.c_str(), */
					logMsg.threadId,
					dflog::os::filename(logMsg.srcLoc.filename).c_str(),
					logMsg.srcLoc.line,
					logMsg.logMsg.c_str()
					));

			return ;
		}

		void FormatHelper::format_f(const LogMsg_T &logMsg, formatBuf_t &buf)
		{
			::memset(logBuf_, 0x00, maxLogBufLen_);
			if (lastSec_ != logMsg.time.sec)
			{
				this->updateTime_(logMsg.time.sec);
			}
			logBufLen_ = std::snprintf(logBuf_, maxLogBufLen_, "[%s.%03ld] [%s] [%lu] [%s](%d): %s\n",
					lastTime_,
					logMsg.time.usec / 1000,
					dflog::level::LEVEL[logMsg.level],
					/* logMsg.logName.c_str(), */
					logMsg.threadId,
					dflog::os::filename(logMsg.srcLoc.filename).c_str(),
					logMsg.srcLoc.line,
					logMsg.logMsg.c_str()
					);

			buf += (logBuf_);
			return ;
		}

		void FormatHelper::formatToString_f(std::string &buf, const char *fmt, va_list &ap)
		{
			// va_list ap;

			// va_start(ap, fmt)
			char s[4096] = {};
			std::vsnprintf(s, sizeof(s), fmt, ap);
			// va_end(ap);
			buf += s;
		}

		void FormatHelper::updateTime_(time_t sec)
		{
			struct tm localtm;
			::localtime_r(&sec, &localtm);
			lastSec_ = sec;
			::memset(lastTime_, 0x00, sizeof(lastTime_));
			::strftime(lastTime_, sizeof(lastTime_), "%Y-%m-%d %H:%M:%S", &localtm);
		}


	}; /* namespace fmtHelper end */

}; /* namespace dflog end */
