//***************************************************************
// @file:    sink.h
// @author:  dingfang
// @date    2020-06-11 19:23:47
//***************************************************************

#pragma once

#include <dflog/common.h>
#include <dflog/formatHelper.h>

#include <string>
#include <memory>


namespace dflog
{

    namespace sinks
    {
        class Sink
        {
        public:
            Sink() = default;
            virtual ~Sink() = default;

            virtual void log(const LogMsg_T &msg) = 0;
            virtual void flush() = 0;
            virtual void setPattern(const std::string &pattern) = 0;
            virtual void setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper) = 0;

            void setLevel(level::Level_E level) { level_ = level; }
            void setFlushLevel(level::Level_E level) { flushLevel_ = level; }
            level::Level_E level() const { return level_; }
            bool shouldLog(level::Level_E level) const { return (level_ <= level); }
            bool shouldFlush(level::Level_E level) const { return (flushLevel_ <= level); }

            Sink(const Sink &) = delete;
            Sink(Sink &&) = delete;
            Sink &operator =(const Sink &) = delete;
            Sink &operator =(Sink &&) = delete;

        protected:
            dflog::level::Level_E level_ = dflog::level::DEBUG;
            dflog::level::Level_E flushLevel_ = dflog::level::TRACE;

            std::unique_ptr<dflog::fmtHelper::FormatHelper> formatHelper_;
            Mutex mutex_;
        };


    }; /* namepspace sinks end */
}; /* namespace dflog end */
