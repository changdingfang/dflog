//***************************************************************
// @file:    dflog.h
// @author:  dingfang
// @date    2020-06-11 18:22:52
//***************************************************************

#pragma once

#include <dflog/logger.h>


using namespace dflog::level;

#define LOG(level, ...)  (dflog::Logger::Instance())->log(dflog::SrcLoc_T{__FILE__, __LINE__, __FUNCTION__}, level, __VA_ARGS__)
#define LOGF(level, ...)  (dflog::Logger::Instance())->logf(dflog::SrcLoc_T{__FILE__, __LINE__, __FUNCTION__}, level, __VA_ARGS__)

namespace dflog
{


    /*
     * 初始化日志
     * */
    static void InitLog(std::string filename, dflog::loggerOption::Option_t logOption = dflog::loggerOption::FILELOG, dflog::Method method = dflog::Method::SYNC)
    {
        dflog::Logger::Instance()->initLog(filename.c_str(), logOption, method);
    }


    /*
     * 设置日志等级
     * */
    static void SetLevel(dflog::level::Level_E lvl, dflog::loggerOption::Option_t logOption = dflog::loggerOption::ALL_SINKS)
    {
        dflog::Logger::Instance()->setLevel(lvl, logOption) /* default DEBUG */;
    }


    /*
     * 设置写入文件刷新等级
     * 当达到该等级时进行刷新写入文件(或达到最大缓存自动写入)
     * */
    static void SetFlushLevel(dflog::level::Level_E lvl, loggerOption::Option_t logOption = loggerOption::ALL_SINKS)
    {
        dflog::Logger::Instance()->setFlushLevel(lvl, logOption);
    }


    /*
     * 刷新缓冲区
     * */
    static void Fflush()
    {
        dflog::Logger::Instance()->fflush();
    }


    /* 
     * 设置单个日志文件最大值
     * 达到最大值后将分割日志
     * default 100MB 
     * */
    static void SetFileSize(uint64_t filesize)
    {
        dflog::Logger::Instance()->setFileSize(filesize);
    }


    /*
     * 设置每日分割日志时间, 24小时制
     * default 06:30
     * */
    static bool SetRotationTime(int hour, int min)
    {
        return dflog::Logger::Instance()->setRotationTime(hour, min);
    }


    /**/
    static void SetFiles()
    {
    }


    /*
     * 终端日志颜色打印开关
     * */
    static void SetTerminalColor(bool shouldColor)
    {
        dflog::Logger::Instance()->setTerminalColor(shouldColor);
    }


}; /* namespace dflog end */
