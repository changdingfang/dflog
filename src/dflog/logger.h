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

	enum class Method: uint8_t
	{
		SYNC	= 1,
		ASYNC	= 2
	};


	class AsyncLogger
	{
	public:
		AsyncLogger();
		AsyncLogger(uint32_t maxSize);
		~AsyncLogger();

		inline void stop()	{	shouldStop_ = true;	}
		inline bool isStop()	{	return shouldStop_;	}

		inline void push(LogMsg_T logMsg)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			cv_.wait(lock, [this] { return (!this->msgCricle_.full()); });
			msgCricle_.push(std::move(logMsg));
			cv_.notify_one();
		}

		inline CricleQueue<LogMsg_T> &getMsgCricle()
		{	return msgCricle_;	}

		static void log(AsyncLogger *p);

	private:
		static std::mutex mutex_;
		static std::condition_variable cv_;
		std::thread logThread_;
		CricleQueue<LogMsg_T> msgCricle_;
		bool shouldStop_ = false;
	};


	class Logger
	{
	public:
		static Logger *Instance()
		{
			if (pLogger_ == nullptr)
			{
				pthread_once(&once_, newLogger_);
			}
			return pLogger_;
		}

		bool initLog(const char *filename, loggerOption::Option_t option = loggerOption::FILELOG, dflog::Method method = dflog::Method::SYNC);

		void log(SrcLoc_T srcLoc, level::Level_E level, const char *fmt, ...) __attribute__((format(printf, 4,5)));

		void setLevel(level::Level_E level, loggerOption::Option_t option = loggerOption::ALL_SINKS);
		void setFlushLevel(level::Level_E level, loggerOption::Option_t option = loggerOption::ALL_SINKS);

		void fflush();

	private:
		friend void AsyncLogger::log(AsyncLogger *p);

		class Destroy
		{
		public:
			Destroy() {};
			~Destroy()
			{
				if (pLogger_ != nullptr)
				{
					delete pLogger_;
					pLogger_ = nullptr;
				}
			}
		};

		Logger() = default;
		~Logger() = default;

		void sinkIt_(LogMsg_T logMsg);

		static void newLogger_()
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

		std::shared_ptr<AsyncLogger> asyncLogger_;
		dflog::Method loggerMethod_ = dflog::Method::SYNC;

		Mutex mutex_;
		bool shouldInit_ = true;

		static Destroy destroy_;
	};

};/* dflog namespace end */
