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

		class FormatHelper
		{
		public:
			FormatHelper();
			~FormatHelper();
			void format(const LogMsg_T &logMsg, formatBuf_t &buf);

			static void formatToString(std::string &buf, const char *fmt, va_list &ap);

		private:
			const uint32_t maxLogBufLen_ = 4096;
			char *logBuf_ = nullptr;
			uint32_t logBufLen_ = 0;

			char lastTime_[32] = {};
			time_t lastSec_ = 0;

		};

	}; /* namespace fmtHelper end */

};/* namespace dflog end */
