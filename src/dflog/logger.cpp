//***************************************************************
// @file:    logger.cpp
// @author:  dingfang
// @date    2020-06-13 22:24:37
//***************************************************************

#include <dflog/logger.h>
#include <dflog/normalSink.h>
#include <dflog/formatHelper.h>

#include <stdarg.h>
#include <sys/time.h>

namespace dflog
{


	Logger *Logger::pLogger_ = nullptr;
	pthread_once_t Logger::once_ = PTHREAD_ONCE_INIT;

	bool Logger::initLog(const char *filename, loggerOption::Option_t option)
		/* Option = FILELOG */
	{
		if (!shouldInit_)
		{
			return false;
		}

		{
			std::lock_guard<dflog::Mutex> lock(mutex_);
			if (!shouldInit_)
			{
				return false;
			}
		}
		shouldInit_ = false;

		if (option & loggerOption::FILELOG)
		{
			/* 默认每天 06:00 时刻分割日志 */
			/* 默认分割日志大小为 100MB */
			/* 默认分割日志文件数量上限为 30个 */
			dflog::sinks::Rotation_T rt;
			rt.rotationH = 6;
			rt.rotationM = 0;
			sinkPtr sp(new sinks::NormalSink(filename, rt, 100 * 1024 * 1024, 30));
			sp->setFormatter(std::move(std::unique_ptr<dflog::fmtHelper::FormatHelper>(new dflog::fmtHelper::FormatHelper())));
			sinks_.push_back(std::make_pair(loggerOption::FILELOG, sp));
		}
		if (option & loggerOption::CONSOLE)
		{
			/* ... */
		}
		this->setLevel(dflog::level::DEBUG);

		return true;
	}

	void Logger::log(SrcLoc_T srcLoc, level::Level_E level, const char *fmt, ...)
	{
		std::string msg;
		va_list ap;
		va_start(ap, fmt);
		dflog::fmtHelper::FormatHelper::formatToString(msg, fmt, ap);
		va_end(ap);

		struct timeval tv;
		gettimeofday(&tv, nullptr);
		LogMsg_T logMsg(tv, srcLoc, "dflog", level, msg);
		for (auto &sinkPair : sinks_)
		{
			if (sinkPair.second->shouldLog(level))
			{
				sinkPair.second->log(logMsg);
			}
		}

		return ;
	}

	void Logger::setLevel(level::Level_E level, loggerOption::Option_t option)
		/* default option = loggerOption::ALL_SINKS */
	{
		for (auto &sinkPair : sinks_)
		{
			if (sinkPair.first & option)
			{
				sinkPair.second->setLevel(level);
			}
		}

		return ;
	}
	void Logger::setFlushLevel(level::Level_E level, loggerOption::Option_t option)
	{
		for (auto &sinkPair : sinks_)
		{
			if (sinkPair.first & option)
			{
				sinkPair.second->setFlushLevel(level);
			}
		}

		return ;
	}
	
	void Logger::fflush()
	{
		for (auto &sinkPair : sinks_)
		{
			sinkPair.second->flush();
		}
	}


};/* dflog namespace end */
