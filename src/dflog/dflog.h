//***************************************************************
// @file:    dflog.h
// @author:  dingfang
// @date    2020-06-11 18:22:52
//***************************************************************

#pragma once

#include <dflog/logger.h>


using namespace dflog::level;

#define LOG(level, ...)  (dflog::Logger::Instance())->log(dflog::SrcLoc_T{__FILE__, __LINE__, __FUNCTION__}, level, __VA_ARGS__)


namespace dflog
{
	static void InitLog(std::string filename, dflog::loggerOption::Option_t logOption = dflog::loggerOption::FILELOG, dflog::Method method = dflog::Method::SYNC)
	{
		dflog::Logger::Instance()->initLog(filename.c_str(), logOption, method);
	}

	static void SetLevel(dflog::level::Level_E lvl, dflog::loggerOption::Option_t logOption = dflog::loggerOption::FILELOG)
	{
		dflog::Logger::Instance()->setLevel(lvl, logOption) /* default DEBUG */;
	}

	static void SetFlushLevel(dflog::level::Level_E lvl, loggerOption::Option_t logOption = loggerOption::ALL_SINKS)
	{
		dflog::Logger::Instance()->setFlushLevel(lvl, logOption);
	}

	/* default 100MB */
	static void SetFileSize(uint64_t filesize)
	{
		// dflog::Logger::Instance()->setFileSize(filesize);
	}

	/*
	 * 
	 * default 06:30
	 * */
	static void SetRotationTime(int hour, int min)
	{
		// dflog::Logger::Instance()->setRotationTime(hour, min);
	}

	/**/
	static void SetFiles()
	{
	}


}; /* namespace dflog end */
