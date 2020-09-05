//***************************************************************
// @file:    logger.cpp
// @author:  dingfang
// @date    2020-06-13 22:24:37
//***************************************************************

#include <dflog/logger.h>
#include <dflog/normalSink.h>
#include <dflog/consoleSink.h>
#include <dflog/formatHelper.h>

#include <stdarg.h>
#include <sys/time.h>


namespace dflog
{

    std::mutex AsyncLogger::mutex_;
    std::condition_variable AsyncLogger::cv_;

    AsyncLogger::AsyncLogger()
        : logThread_(std::thread(AsyncLogger::log, this))
    { }

    AsyncLogger::AsyncLogger(uint32_t maxSize)
        : msgCricle_(maxSize)
          , logThread_(std::thread(AsyncLogger::log, this))
    { }

    AsyncLogger::~AsyncLogger()
    {
        shouldStop_ = true;
        logThread_.join();
    }

    void AsyncLogger::log(AsyncLogger *p)
    {
        if (p == nullptr)
        {
            throwDflogEx("async logger is null");
        }

        while (!p->isStop() || !p->getMsgCricle().empty())
        {
            std::unique_lock<std::mutex> lock(AsyncLogger::mutex_);
            if (!AsyncLogger::cv_.wait_for(lock, std::chrono::seconds(1), [p]{ return (!p->getMsgCricle().empty()); }))
            {
                continue;
            }

            Logger::Instance()->sinkIt_(std::move(p->getMsgCricle().pop()));
            cv_.notify_one();
        }

        return ;
    }


    Logger *Logger::pLogger_ = nullptr;
    pthread_once_t Logger::once_ = PTHREAD_ONCE_INIT;
    Logger::Destroy Logger::destroy_;

    bool Logger::initLog(const char *filename, loggerOption::Option_t option, dflog::Method method)
        /* default Option = FILELOG, Method = SYNC */
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
            shouldInit_ = false;
        }

        loggerMethod_ = method;
        if (loggerMethod_ == dflog::Method::ASYNC)
        {
            asyncLogger_ = std::make_shared<AsyncLogger>(10 * 1000);
        }

        if (option & loggerOption::FILELOG)
        {
            /* 默认每天 06:00 时刻分割日志 */
            /* 默认分割日志大小为 100MB */
            /* 默认分割日志文件数量上限为 30个 */
            dflog::sinks::Rotation_T rt{6, 0};
            sinkPtr sp(new sinks::NormalSink(filename, rt, 100 * 1024 * 1024, 30));
            sp->setFormatter(std::move(std::unique_ptr<dflog::fmtHelper::FormatHelper>(new dflog::fmtHelper::FormatHelper())));
            sinks_.push_back(std::make_pair(loggerOption::FILELOG, sp));
        }
        if (option & loggerOption::CONSOLE)
        {
            sinkPtr sp(new sinks::ConsoleSink());
            sp->setFormatter(std::move(std::unique_ptr<dflog::fmtHelper::FormatHelper>(new dflog::fmtHelper::FormatHelper())));
            sinks_.push_back(std::make_pair(loggerOption::CONSOLE, sp));
        }

        this->setLevel(dflog::level::DEBUG, loggerOption::ALL_SINKS);

        return true;
    }

    void Logger::logf(SrcLoc_T srcLoc, level::Level_E level, const char *fmt, ...)
    {
        std::string msg;
        va_list ap;
        va_start(ap, fmt);
        dflog::fmtHelper::FormatHelper::formatToString_f(msg, fmt, ap);
        va_end(ap);
        this->log_(srcLoc, level, msg);
    }


    void Logger::setLevel(level::Level_E level, loggerOption::Option_t option)
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


    void Logger::setFileSize(uint64_t filesize)
    {
        for (auto &sinkPair : sinks_)
        {
            if (sinkPair.first & dflog::loggerOption::FILELOG)
            {
                dynamic_cast<sinks::NormalSink *>(sinkPair.second.get())->setFileSize(filesize);
            }
        }
    }


    bool Logger::setRotationTime(int hour, int min)
    {
        if (hour > 23 || hour < 0
                || min > 59 || min < 0)
        {
            return false;
        }
        dflog::sinks::Rotation_T rt{hour, min};
        for (auto &sinkPair : sinks_)
        {
            if (sinkPair.first & dflog::loggerOption::FILELOG)
            {
                dynamic_cast<sinks::NormalSink *>(sinkPair.second.get())->setRotationTime(rt);
            }
        }
        return true;
    }


    void Logger::setTerminalColor(bool shouldColor)
    {
        for (auto &sinkPair : sinks_)
        {
            if (sinkPair.first & dflog::loggerOption::CONSOLE)
            {
                dynamic_cast<sinks::ConsoleSink *>(sinkPair.second.get())->setTerminalColor(shouldColor);
                break;
            }
        }
    }


    void Logger::fflush()
    {
        for (auto &sinkPair : sinks_)
        {
            sinkPair.second->flush();
        }
    }


    void Logger::log_(SrcLoc_T srcLoc, level::Level_E level, std::string msg)
    {
        struct timeval tv;
        ::gettimeofday(&tv, nullptr);
        LogMsg_T logMsg(tv, std::move(srcLoc), std::move("dflog"), level, std::move(msg));

        if (loggerMethod_ == Method::ASYNC)
        {
            asyncLogger_->push(std::move(logMsg));
            return ;
        }
        this->sinkIt_(std::move(logMsg));
        return ;
    }


    void Logger::sinkIt_(LogMsg_T logMsg)
    {
        for (auto &sinkPair : sinks_)
        {
            if (sinkPair.second->shouldLog(logMsg.level))
            {
                sinkPair.second->log(std::move(logMsg));
            }
        }

    }


};/* dflog namespace end */
