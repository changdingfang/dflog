//***************************************************************
// @file:    common.h
// @author:  dingfang
// @date    2020-06-11 19:33:00
//***************************************************************

#pragma once

#include <pthread.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include <string>
#include <chrono>
#include <thread>
#include <mutex>

namespace dflog
{
#define DFLOG_LOG_BACKUP_FORMAT "%Y-%m-%d"

#define DFLOG_GET_THREAD_ID 186
#define ONE_DAY_SEC 86400

	using formatBuf_t = std::string;

	class DflogEx : public std::exception
	{
	public:
		explicit DflogEx(std::string msg);
		DflogEx(const std::string &msg, int lastErrno);
		const char *what() const noexcept override;

	private:
		std::string msg_;
	};

	void throwDflogEx(const std::string &msg, int lastErrno);
	void throwDflogEx(std::string msg);
	

	namespace os
	{
		
		inline int pid() noexcept
		{
			return static_cast<int>(::getpid());
		}

		inline size_t threadId() noexcept
		{
			// return static_cast<size_t>(pthread_self());
			return static_cast<size_t>(::syscall(DFLOG_GET_THREAD_ID));
		}


		inline int rename(const std::string &srcName, const std::string &dstName)
		{
			return (std::rename(srcName.c_str(), dstName.c_str()));
		}

		inline size_t filesize(FILE *pf)
		{
			if (pf == nullptr)
			{
				throwDflogEx("get filesize failed! FILE pointer is null!");
			}
			int fd = ::fileno(pf);
			struct stat64 st;
			if (::fstat64(fd, &st) == 0)
			{
				return static_cast<size_t>(st.st_size);
			}

			throwDflogEx("get filesize error!", errno);
		}


		inline bool pathExists(const std::string &path)
		{
			struct stat buffer;
			return (stat(path.c_str(), &buffer) == 0);
		}

		bool createDir(std::string path);
		inline std::string filename(const std::string &filename)
		{
			std::string::size_type pos = filename.find_last_of('/');
			return pos != std::string::npos ? filename.substr(pos + 1, filename.size()) : filename;
		}
		inline std::string filename(const char *filename)
		{
			std::string s(filename);
			std::string::size_type pos = s.find_last_of('/');
			return pos != std::string::npos ? s.substr(pos + 1, s.size()) : filename;
		}
		inline std::string dirname(const std::string &path)
		{
			std::string::size_type pos = path.find_last_of('/');

			return (pos != std::string::npos ? path.substr(0, pos) : std::string(""));
		}


	}; /* namespace os end */




	namespace level
	{

		typedef enum E_Level
		{
			TRACE	= 1,
			DEBUG	= 2,
			INFO	= 3,
			WARN	= 4,
			ERROR	= 5,
			CRITICAL= 6,
			LOG_OFF		= 7
		}Level_E;

		const char LEVEL[][10] = {"NONE", "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL", "OFF"};

	}; /* namespace level end */

	typedef struct T_LogClock
	{
		T_LogClock() = default;
		constexpr T_LogClock(time_t secIn, suseconds_t usecIn)
			: sec(secIn)
			, usec(usecIn)
		{	}
		T_LogClock(const struct timeval &tv)
		{
			this->sec	= tv.tv_sec;
			this->usec	= tv.tv_usec;
		}
		bool operator > (const T_LogClock &other) const
		{
			if (this->sec > other.sec || 
				(this->sec == other.sec && this->usec > other.usec))
			{
				return true;
			}
			return false;
		}

		time_t sec;
		suseconds_t usec;
	}LogClock_T;

	typedef struct T_SrcLoc
	{
		constexpr T_SrcLoc() = default;
		constexpr T_SrcLoc(const char *filenameIn, int lineIn, const char *funcnameIn)
			: filename(filenameIn)
			  , line(lineIn)
			  , funcname(funcnameIn)
		{	}

		constexpr bool empty() const noexcept {	return (line == 0);	}

		const char *filename = nullptr;
		int line = 0;
		const char *funcname = nullptr;
	}SrcLoc_T;

	typedef struct T_LogMsg
	{
		T_LogMsg() = default;
		T_LogMsg(LogClock_T logTime, SrcLoc_T loc, std::string logName, level::Level_E lv, std::string msg)
		: logName(logName)
		, level(lv)
		, time(logTime)
		, threadId(os::threadId())
		, srcLoc(loc)
		, logMsg(msg)
		{};

		std::string logName;
		level::Level_E level = level::LOG_OFF;
		LogClock_T time;
		size_t threadId = 0;

		/* * * * */
		/* color */
		/* * * * */

		SrcLoc_T srcLoc;
		std::string logMsg;
	}LogMsg_T;

	class Mutex
	{
	public:
		Mutex() = default;
		~Mutex() = default;
		void lock() 
		{
			mutex_.lock();
		};
		void tryLock() 
		{
			mutex_.try_lock();
		};

		void unlock() 
		{
			mutex_.unlock();
		};
	private:
		std::mutex mutex_;
	};

}; /* namespace common end */
