//***************************************************************
// @file:    common.h
// @author:  dingfang
// @date    2020-06-11 19:33:00
//***************************************************************

#pragma once

#include <pthread.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <map>

#define DFLOG_LOG_BACKUP_FORMAT "%Y-%m-%d"
#define ONE_DAY_SEC 86400

namespace dflog
{

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

        inline int pid() noexcept { return static_cast<int>(::getpid()); }
        inline size_t threadId() noexcept 
        { return static_cast<size_t>(::syscall(__NR_gettid)); }


        inline int rename(const std::string &srcName, const std::string &dstName)
        { return std::rename(srcName.c_str(), dstName.c_str()); }

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
                return st.st_size;
            }

            throwDflogEx("get filesize error!", errno);
            return 0;
        }


        inline bool pathExists(const std::string &path)
        {
            struct stat buffer;
            return (stat(path.c_str(), &buffer) == 0);
        }

        bool createDir(std::string path);

        inline std::string filename(const std::string &fn)
        {
            std::string::size_type pos = fn.find_last_of('/');
            return pos != std::string::npos ? fn.substr(pos + 1, fn.size()) : fn;
        }

        inline std::string filename(const char *fn)
        {
            const std::string s(fn);
            return filename(s);
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
            TRACE       = 1,
            DEBUG       = 2,
            INFO        = 3,
            WARN        = 4,
            ERROR       = 5,
            CRITICAL    = 6,
            LOG_OFF     = 7
        }Level_E;

        constexpr char LEVEL[][10] = {"NONE", "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL", "OFF"};


    }; /* namespace level end */

    namespace terminalcolor
    {

        enum class ATTR_E : uint8_t
        {
            NORMAL = 0,
            BLOD,
            REVERSE = 7,
        };

        enum class FG_E : uint8_t
        {
            BLACK = 30,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            PURPLE,
            CYAN,
            WHITE
        };

        enum class BG_E : uint8_t
        {
            BLACK = 40,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            PURPLE,
            CYAN,
            WHITE,
        };

        typedef struct T_TerminalColor
        {
            ATTR_E attr;
            FG_E fg;
            BG_E bg;
        }TerminalColor_T;

        static std::map< level::Level_E, TerminalColor_T > TerminalColor = {
            { level::TRACE, { ATTR_E::BLOD, FG_E::BLACK, BG_E::BLACK } },
            { level::DEBUG, { ATTR_E::NORMAL, FG_E::WHITE, BG_E::BLACK } },
            { level::INFO, { ATTR_E::NORMAL, FG_E::GREEN, BG_E::BLACK } },
            { level::WARN, { ATTR_E::NORMAL, FG_E::PURPLE, BG_E::BLACK } },
            { level::ERROR, { ATTR_E::NORMAL, FG_E::RED, BG_E::BLACK } },
            { level::CRITICAL, { ATTR_E::REVERSE, FG_E::RED, BG_E::BLACK } }
        };

    }; /* namespace terminalcolor end */

    typedef struct T_LogClock
    {
        constexpr T_LogClock(time_t secIn, suseconds_t usecIn)
            : sec(secIn)
              , usec(usecIn)
        { }

        T_LogClock(const struct timeval &tv)
            : sec(tv.tv_sec)
              , usec(tv.tv_usec)
        { }

        bool operator > (const T_LogClock &other) const noexcept
        {
            return this->sec > other.sec || 
                (this->sec == other.sec && this->usec > other.usec);
        }

        time_t sec;
        suseconds_t usec;
    }LogClock_T;

    typedef struct T_SrcLoc
    {
        constexpr T_SrcLoc(const char *filenameIn, int lineIn, const char *funcnameIn)
            : filename(filenameIn)
              , line(lineIn)
              , funcname(funcnameIn)
        { }

        constexpr bool empty() const noexcept { return (line == 0); }

        const char *filename = nullptr;
        int line = 0;
        const char *funcname = nullptr;
    }SrcLoc_T;

    typedef struct T_LogMsg
    {
        T_LogMsg() = default;
        T_LogMsg(LogClock_T logTime, SrcLoc_T loc, std::string logName, level::Level_E lv, std::string msg)
            : logName(std::move(logName))
              , level(lv)
              , time(std::move(logTime))
              , threadId(os::threadId())
              , srcLoc(std::move(loc))
              , logMsg(std::move(msg))
        {};

        std::string logName;
        level::Level_E level = level::LOG_OFF;
        LogClock_T time {0, 0};
        size_t threadId = 0;
        SrcLoc_T srcLoc {nullptr, 0, nullptr};
        std::string logMsg;
    }LogMsg_T;

    class Mutex
    {
    public:
        Mutex() = default;
        ~Mutex() = default;

        inline void lock()    { mutex_.lock(); };
        inline void tryLock() { mutex_.try_lock(); };
        inline void unlock()  { mutex_.unlock(); };

    private:
        std::mutex mutex_;
    };


    template<typename T>
        class CricleQueue
        {
        public:
            CricleQueue() { data_.resize(maxSize_); }
            CricleQueue(uint32_t maxSize)
                : maxSize_(maxSize)
            {    data_.resize(maxSize_);    }
            ~CricleQueue()
            {    data_.clear();    }

            inline bool empty() const    { return tail_ == head_; }
            inline bool full() const     { return (tail_ + 1) % maxSize_ == head_; }
            inline uint32_t size() const { return size_; }

            inline bool push(T msg)
            {
                if (this->full())
                {
                    return false;
                }
                data_[tail_] = std::move(msg);
                tail_ = (tail_ + 1) % maxSize_;
                ++size_;
                return true;
            }

            inline T pop()
            {    
                if (this->empty())
                {
                    throwDflogEx("queue is empty!");
                }
                uint32_t curr = head_;
                head_ = (head_ + 1) % maxSize_;
                --size_;

                return std::move(data_[curr]);
            }

        private:
            std::vector<T> data_;
            uint32_t head_ = 0;
            uint32_t tail_ = 0;
            uint32_t size_ = 0;
            const uint32_t maxSize_ = 10000;
        };


}; /* namespace common end */
