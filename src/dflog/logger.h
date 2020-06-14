//***************************************************************
// @file:    logger.h
// @author:  dingfang
// @date    2020-06-11 18:21:29
//***************************************************************


#pragma once

#include <dflog/sink.h>
#include <dflog/common.h>

#include <vector>
#include <utility>

namespace dflog
{
	using sinkPtr = std::shared_ptr<sinks::Sink>;
	namespace loggerOption
	{
		using Option_t = uint16_t;
		constexpr Option_t CONSOLE	= 1;
		constexpr Option_t FILELOG	= 2;
		constexpr Option_t ALL_SINKS = 0xFFFF;
	}; /* namespace loggerOption end */


	class Logger
	{
	public:
		static Logger *Instance()
		{
			if (pLogger_ == nullptr)
			{
				pthread_once(&once_, newLogger);
			}
			return pLogger_;
		}

		bool initLog(const char *filename, loggerOption::Option_t option = loggerOption::FILELOG);

		void log(SrcLoc_T srcLoc, level::Level_E level, const char *fmt, ...);

		void setLevel(level::Level_E level, loggerOption::Option_t option = loggerOption::ALL_SINKS);
		void setFlushLevel(level::Level_E level, loggerOption::Option_t option = loggerOption::ALL_SINKS);

		void fflush();

	private:
		Logger() = default;
		~Logger() = default;

		static void newLogger()
		{
			if (pLogger_ == nullptr)
			{
				pLogger_ = new Logger();
			}
		}
	private:
		static Logger *pLogger_;
		static pthread_once_t once_;
		std::vector< std::pair< loggerOption::Option_t, sinkPtr > > sinks_;

		Mutex mutex_;
		bool shouldInit_ = true;
	};

};/* dflog namespace end */
