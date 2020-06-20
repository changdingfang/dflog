 //***************************************************************
 // @file:    testPerformanceTest.cpp
 // @author:  dingfang
 // @date    2020-06-14 11:32:31
 //***************************************************************
 
#include <dflog/dflog.h>
#include <sys/time.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <thread>
#include <stack>


using namespace std;
using namespace dflog;
// using namespace dflog::level;

constexpr uint32_t ONE_MILLION = 1000000;

string commaNum(uint32_t num)
{
	stack<string> s;
	uint16_t count = 0;
	while (num / 10 > 0)
	{
		s.push(std::to_string(num % 10));
		num /= 10;
		if (++count % 3 == 0)
		{
			s.push(",");
		}
	}
	string numStr("");
	numStr.append(std::to_string(num));
	while (!s.empty())
	{
		numStr.append(s.top());
		s.pop();
	}

	return std::move(numStr);
}


void test(int32_t lines)
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	for (int i = 0; i < lines; ++i)
	{
		LOGF(INFO, "hello world! (%d)", i);
	}
	struct timeval tv2;
	gettimeofday(&tv2, nullptr);

	double use = ((tv2.tv_sec - tv.tv_sec) * ONE_MILLION  + (tv2.tv_usec - tv.tv_usec)) * 1.0 / ONE_MILLION;
	printf("lines:\t\t(%s)\n", commaNum(lines).c_str());
	printf("use time:\t%.3lf (s)\n", use);
	printf("speed:\t\t%s (line/s)\n", commaNum(static_cast<uint32_t>(lines / use)).c_str());
	printf("speed:\t\t%.3lf (us/line)\n", (use * ONE_MILLION) / lines);

	return ;
}


void singleProcess(uint32_t logLines)
{
	printf("* * * * * * * * * * * * * * * * * * *\n");
	printf("* * * * single process  * * * * * * *\n");
	test(logLines);
	printf("* * * * * * * * * * * * * * * * * * *\n\n");

	return ;
}

void multithread(uint32_t logLines, uint32_t threadNum)
{

	struct timeval tv;
	gettimeofday(&tv, nullptr);

	vector<thread> t;
	for (uint32_t i = 0; i < threadNum; ++i)
	{
		t.emplace_back([logLines, threadNum]() {
					for (uint32_t j = 0; j < logLines / threadNum; ++j)
					{
					LOGF(INFO, "hello world! (%d)", j);
					}
					});
	}

	for (auto &v : t)
	{
		v.join();
	}

	struct timeval tv2;
	gettimeofday(&tv2, nullptr);

	double use = ((tv2.tv_sec - tv.tv_sec) * ONE_MILLION + (tv2.tv_usec - tv.tv_usec)) * 1.0 / ONE_MILLION;
	printf("* * * * * * * * * * * * * * * * * * *\n");
	printf("* * * * * * multithread * * * * * * *\n");
	printf("thread num:\t(%s)\n", commaNum(threadNum).c_str());
	printf("lines:\t\t(%s)\n", commaNum(logLines).c_str());
	printf("use time:\t%.3lf (s)\n", use);
	printf("speed:\t\t%s (line/s)\n", commaNum(static_cast<uint32_t>(logLines / use)).c_str());
	printf("speed:\t\t%.3lf (us/line)\n", (use * ONE_MILLION / logLines));

	return ;
	printf("* * * * * * * * * * * * * * * * * * *\n");

	return ;
}


int main(void)
{
	dflog::InitLog("cdf_test.log", loggerOption::FILELOG | loggerOption::CONSOLE, dflog::Method::SYNC);
	dflog::SetLevel(TRACE);
	dflog::SetFlushLevel(LOG_OFF);
	
	constexpr uint32_t lines =  10 * 10000;
	singleProcess(lines);
	multithread(lines, 2);

	dflog::Fflush();

	return 0;
}
