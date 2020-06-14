//***************************************************************
// @file:    formatHelper.cpp
// @author:  dingfang
// @date    2020-06-13 10:48:53
//***************************************************************



#include <dflog/formatHelper.h>

#include <string.h>
#include <stdio.h>

namespace dflog
{
	namespace fmtHelper
	{

		FormatHelper::FormatHelper()
		{
			logBuf_ = new char[maxLogBufLen_]();
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
			memset(logBuf_, 0x00, maxLogBufLen_);
			struct tm localtm;
			localtime_r(&logMsg.time.sec, &localtm);
			logBufLen_ = sprintf(logBuf_, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] [%s] [%ld] [%s](%d): ",
					localtm.tm_year + 1900, 
					localtm.tm_mon + 1, 
					localtm.tm_mday,
					localtm.tm_hour,
					localtm.tm_min,
					localtm.tm_sec,
					logMsg.time.usec / 1000,
					dflog::level::LEVEL[logMsg.level],
					/* logMsg.logName.c_str(), */
					logMsg.threadId,
					dflog::os::filename(logMsg.srcLoc.filename).c_str(),
					logMsg.srcLoc.line
					);
			if (logBufLen_ + logMsg.logMsg.size() < maxLogBufLen_)
			{
				memcpy(logBuf_ + logBufLen_, logMsg.logMsg.c_str(), logMsg.logMsg.size());
				logBuf_[logBufLen_ + logMsg.logMsg.size()] = '\n';
			}

			buf = formatBuf_t(logBuf_);
			return ;
		}

		void FormatHelper::formatToString(std::string &buf, const char *fmt, va_list &ap)
		{
			// va_list ap;

			// va_start(ap, fmt)
			char s[4096] = {};
			vsnprintf(s, sizeof(s), fmt, ap);
			// va_end(ap);
			buf = std::move(std::string(s));
		}

	}; /* namespace fmtHelper end */

}; /* namespace dflog end */
