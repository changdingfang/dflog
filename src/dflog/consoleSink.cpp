//***************************************************************
// @file:    consoleSink.cpp
// @author:  dingfang
// @date    2020-06-20 11:19:22
//***************************************************************

#include <dflog/consoleSink.h>
#include <dflog/common.h>


namespace dflog
{

    namespace sinks
    {


        void ConsoleSink::log(const dflog::LogMsg_T &msg)
        {
            std::lock_guard<dflog::Mutex> lock(Sink::mutex_);

            formatBuf_t formattedMsg;
            /* format */
            Sink::formatHelper_->format_f(msg, formattedMsg);

            /* console out */
            if (shouldColor_)
            {
                terminalcolor::TerminalColor_T tc(terminalcolor::TerminalColor[msg.level]);
                std::fprintf(stdout, "\033[%d;%d;%dm%s\033[0m", 
                        static_cast<int>(tc.attr), 
                        static_cast<int>(tc.fg), 
                        static_cast<int>(tc.bg), 
                        formattedMsg.c_str()); 
                return ;
            }

            std::fprintf(stdout, formattedMsg.c_str());

            return ;
        }


        void ConsoleSink::flush() { return ; }


        void ConsoleSink::setPattern(const std::string &pattern) { return ; }


        void ConsoleSink::setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper)
        {
            Sink::formatHelper_ = std::move(sinkFormatHelper);
            return ;
        }


    }; /* namespace sinks end */

}; /* namespace dflog end */
