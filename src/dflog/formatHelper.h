//***************************************************************
// @file:    formatHelper.h
// @author:  dingfang
// @date    2020-06-12 22:01:38
//***************************************************************

#pragma once

#include <dflog/common.h>

#include <stdarg.h>

namespace dflog
{

    namespace fmtHelper
    {

         
        class FormatHelper final
        {
        public:
            FormatHelper() = default;
            ~FormatHelper() = default;

            void format(const LogMsg_T &logMsg, formatBuf_t &buf);
            void format_f(const LogMsg_T &logMsg, formatBuf_t &buf);

            static void formatToString_f(std::string &buf, const char *fmt, va_list &ap);

        private:
            void updateTime_(time_t sec);

        private:
            static constexpr uint32_t maxLogBufLen_ = 20 * 1024;
            char logBuf_[256] = { '\0' };
            uint32_t logBufLen_ = 0;

            char lastTime_[32] = { '\0' };
            time_t lastSec_ = 0;

        };


    }; /* namespace fmtHelper end */

};/* namespace dflog end */
