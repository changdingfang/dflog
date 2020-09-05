//***************************************************************
// @file:    consoleSink.h
// @author:  dingfang
// @date    2020-06-20 11:07:30
//***************************************************************

#pragma once

#include <dflog/sink.h>


namespace dflog
{

    namespace sinks
    {


        class ConsoleSink final : public Sink
        {
        public:
            ConsoleSink() = default; 
            ~ConsoleSink() = default;

            virtual void log(const LogMsg_T &msg) final;
            virtual void flush() final;
            virtual void setPattern(const std::string &pattern) final;
            virtual void setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper) final;

            inline void setTerminalColor(bool shouldColor) { shouldColor_ = shouldColor; }

        private:
            bool shouldColor_ = true;
        };


    }; /* namespace sinks end */

}; /* namespace dflog end */
