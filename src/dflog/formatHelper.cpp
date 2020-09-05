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
                        logMsg.logMsg.substr(0, maxLogBufLen_).c_str()
                        ));

            return ;
        }

        void FormatHelper::format_f(const LogMsg_T &logMsg, formatBuf_t &buf)
        {
            ::memset(logBuf_, 0x00, logBufLen_);
            if (lastSec_ != logMsg.time.sec)
            {
                this->updateTime_(logMsg.time.sec);
            }

            logBufLen_ = std::sprintf(logBuf_, "[%s.%03ld] [%s] [%lu] [%s](%d): ",
                    lastTime_,
                    logMsg.time.usec / 1000,
                    dflog::level::LEVEL[logMsg.level],
                    /* logMsg.logName.c_str(), */
                    logMsg.threadId,
                    dflog::os::filename(logMsg.srcLoc.filename).c_str(),
                    logMsg.srcLoc.line
                    );

            buf.reserve(logBufLen_ + (logMsg.logMsg.size() % maxLogBufLen_) + 1);
            buf += logBuf_;
            buf += std::move(logMsg.logMsg.substr(0, maxLogBufLen_));
            buf += '\n';

            return ;
        }

        void FormatHelper::formatToString_f(std::string &buf, const char *fmt, va_list &ap)
        {
            char s[maxLogBufLen_] = { '\0' };
            uint32_t size = std::vsnprintf(s, maxLogBufLen_, fmt, ap);
            if (size >= maxLogBufLen_)
            {
                s[maxLogBufLen_] = '\0';
            }
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
