 //***************************************************************
 // @file:    testTerminalColor.cpp
 // @author:  dingfang
 // @date    2020-06-20 11:46:23
 //***************************************************************
 
#include <dflog/dflog.h>

using namespace std;

void testTerminalColor()
{
	dflog::InitLog("", dflog::loggerOption::CONSOLE, dflog::Method::SYNC);
	dflog::SetLevel(TRACE);

	LOG(TRACE, "i am trace log ({}), ({:.2})", 123, 2.3);
	LOG(DEBUG, "i am debug  log ({}), ({})", 123, 3.3123);
	LOG(INFO, "i am debug  log ({:05}), ({:.3})", 123, 3.3123);
	LOG(WARN, "i am warn log [{:#5}]", 100);
	LOG(ERROR, "i am error log {}", "hello wrold");
	LOG(CRITICAL, "i am critical log ");
	LOG(INFO, "* * * * * * * * * * * * * * * * * * * * * * * * ");

	dflog::SetTerminalColor(false);
	LOG(INFO, "* * * * * * * close terminal color print * * * * * * * * * * ");
	LOG(TRACE, "i am trace log ({}), ({:.2})", 123, 2.3);
	LOG(DEBUG, "i am debug  log ({}), ({})", 123, 3.3123);
	LOG(INFO, "i am debug  log ({:05}), ({:.3})", 123, 3.3123);
	LOG(WARN, "i am warn log [{:#5}]", 100);
	LOG(ERROR, "i am error log {}", "hello wrold");
	LOG(CRITICAL, "i am critical log ");
}


int main(void)
{
	testTerminalColor();

	return 0;
}
