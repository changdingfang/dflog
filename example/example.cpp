//***************************************************************
// @file:    example.cpp
// @author:  dingfang
// @date    2020-06-11 18:36:38
//***************************************************************


#include <dflog/dflog.h>


void example()
{
	/* 
	 * 初始化日志
	 * 1. 可以添加不同的定向log输出
	 * 2. 
	 * */
	dflog::Logger::Instance()->initLog("./test.log", dflog::loggerOption::FILELOG | dflog::loggerOption::CONSOLE);
	dflog::Logger::Instance()->setLevel(INFO) /* default DEBUG */;
	/* 不同的log输出, 可以控制不同的日志等级 */
	dflog::Logger::Instance()->setLevel(DEBUG, dflog::loggerOption::FILELOG | dflog::loggerOption::CONSOLE); /* 默认所有日志等级改动 */;
	// Logger::Instance()->setFlushLevel(WARN) /* 默认每次刷新(TRACE) */;

	/* 日志, 使用类似于print输出 */
	LOG(TRACE, "i am trace log (%d), (%.2f)", 123, 2.3);
	LOG(DEBUG, "i am debug  log (%d), (%f)", 123, 3.3123);
	LOG(INFO, "i am info log (%ld)", 123);
	LOG(WARN, "i am warn log %d", 100);
	LOG(ERROR, "i am error log %s", "hello wrold");
	LOG(CRITICAL, "i am critical log ");
	return ;
}


int main(void)
{
	example();

	return 0;
}
