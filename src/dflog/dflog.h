//***************************************************************
// @file:    dflog.h
// @author:  dingfang
// @date    2020-06-11 18:22:52
//***************************************************************

#pragma once

#include <dflog/logger.h>


using namespace dflog::level;

#define LOG(level, ...)  (dflog::Logger::Instance())->log(dflog::SrcLoc_T{__FILE__, __LINE__, __FUNCTION__}, level, __VA_ARGS__)



