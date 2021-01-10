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


        NormalSink::NormalSink(std::string filename, Rotation_T rotation, uint64_t maxFilesize, int16_t maxFiles) 
            /* default maxFilesize = 1024 * 1024 * 1024; maxFiles = -1 */
            : filename_(std::move(filename))
              , rotation_(std::move(rotation))
              , currentSize_(0)
              , maxFilesize_(maxFilesize)
              , maxFiles_(maxFiles)
              , currFiles_(0)
              , rotationTime_(0, 0)
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

            try
            {
                fileHelper_.open(filename_);
            }
            catch(std::string &e)
            {
                ::fprintf(stderr, "open file. %s", e.c_str());
            }
            currentSize_ = fileHelper_.size();

            this->loadLogFilename();
        }


        void NormalSink::log(const dflog::LogMsg_T &msg)
        {
            std::lock_guard<dflog::Mutex> lock(Sink::mutex_);

            formatBuf_t formattedMsg;
            /* format */
            Sink::formatHelper_->format_f(msg, formattedMsg);

            currentSize_ += formattedMsg.size();
            dflog::LogClock_T time(msg.time);
            bool shouldRotate = (msg.time > rotationTime_) || (currentSize_ > maxFilesize_);

            if (shouldRotate)
            {
                this->rotateLog(time.sec);
                while (msg.time > rotationTime_)
                {
                    rotationTime_ .sec += ONE_DAY_SEC;
                }

                currentSize_ = formattedMsg.size();
            }

            /* I/O */
            try
            {
                fileHelper_.write(formattedMsg);
            }
            catch(std::string &e)
            {
                ::fprintf(stderr, "write file. %s", e.c_str());
                return ;
            }

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


        void NormalSink::setFileSize(uint64_t filesize)
        {
            maxFilesize_ = filesize;
        }


        bool NormalSink::setRotationTime(Rotation_T rt)
        {
            rotation_ = rt;
            return true;
        }


        void NormalSink::setFiles(int16_t maxFiles)
        {
            maxFiles_ = maxFiles;
            this->cleanOldFile();
        }


        void NormalSink::rotateLog(time_t time)
        {
            std::string ext, backname;
            std::tie(backname, ext) = FileHelper::splitByExtension(filename_);

            struct tm t;
            ::localtime_r(&time, &t);
            /* 以时间分割, 当文件名相同时, 序号分割. */
            /* 1. filename__year-mon-day.log */
            /*    or */
            /* 2. filename__year-mon-day.log.1 */
            char s[64] = { 0 };
            ::strftime(s, sizeof(s), DFLOG_LOG_BACKUP_FORMAT, &t);
            backname.append("_");
            backname.append(s);
            if (!ext.empty())
            {
                backname.append(ext);
            }

            std::string newBackFilename(backname);
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
                        std::list<std::string> &fnListRef = filenameList_.back();
                        fnListRef.push_back(name);
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
            else
            {
                std::list<std::string> fnList = { backname };
                filenameList_.push_back(fnList);
            }
            ++currFiles_;

            fileHelper_.close();
            dflog::os::rename(filename_, backname);
            try
            {
                fileHelper_.open(filename_);
            }
            catch(std::string &e)
            {
                ::fprintf(stderr, "open file. %s", e.c_str());
            }

            this->cleanOldFile();

            return ;
        }


        void NormalSink::loadLogFilename()
        {
            std::string ext, filepre;
            std::tie(filepre, ext) = FileHelper::splitByExtension(filename_);

            struct timeval tv;
            ::gettimeofday(&tv, nullptr);
            tv.tv_sec += ONE_DAY_SEC;
            while (true)
            {
                struct tm t;
                tv.tv_sec -= ONE_DAY_SEC;
                ::localtime_r(&tv.tv_sec, &t);
                char s[64] = { 0 };
                ::strftime(s, sizeof(s), DFLOG_LOG_BACKUP_FORMAT, &t);
                std::string oldfile(filepre);
                oldfile.append("_");
                oldfile.append(s);
                if (!ext.empty())
                {
                    oldfile.append(ext);
                }
                if (::access(oldfile.c_str(), F_OK) != 0)
                {
                    break;
                }

                std::list<std::string> fnList;
                fnList.push_back(oldfile);

                int num = 0;
                do
                {
                    std::string name(oldfile);
                    name.append(".");
                    name.append(std::to_string(++num));
                    if (::access(name.c_str(), F_OK) != 0)
                    {
                        break;
                    }
                    fnList.push_back(name);
                } while (true);

                currFiles_ += fnList.size();
                filenameList_.push_back(fnList);
            }
        }


        void NormalSink::cleanOldFile()
        {
            if (maxFiles_ == -1)
            {
                return ;
            }

            while (currFiles_ >= maxFiles_)
            {
                std::list<std::string> &fnList = filenameList_.back();
                std::string filename = fnList.back();
                if (::access(filename.c_str(), F_OK) == 0)
                {
                    ::remove(filename.c_str());
                }

                fnList.pop_back();
                if (fnList.empty())
                {
                    filenameList_.pop_back();
                }
                --currFiles_;
            }
        }


    }; /* namespace sinks end */

}; /* namespace dflog end */
