 //***************************************************************
 // @file:    testDflog.cpp
 // @author:  dingfang
 // @date    2020-06-16 21:27:11
 //***************************************************************
 
#include <dflog/dflog.h>

using namespace std;
using namespace dflog;


void testDflog()
{
	/* 
	 * const cahr * 
	 * char *
	 * %s
	 * %c
	 * %d
	 * %ld
	 * %u
	 * %lu
	 * %f
	 * %lf
	 * %p
	 * %x
	 * */

	char s[20] = {"hello world"};
	LOG(INFO, "hello world");
	LOG(INFO, "hello world c_s: %s", "hello world");
	LOG(INFO, "hello world s: %s", s);
	LOG(INFO, "hello world d: %d", 100);
	LOG(INFO, "hello world ld: %ld", static_cast<long int>(100));
	LOG(INFO, "hello world u: %u", static_cast<unsigned int>(100));
	LOG(INFO, "hello world lu: %lu", static_cast<unsigned long int>(100));
	LOG(INFO, "hello world f1: %f", static_cast<float>(100));
	LOG(INFO, "hello world f2: %f", static_cast<float>(100.001));
	LOG(INFO, "hello world lf1: %lf", static_cast<double>(100));
	LOG(INFO, "hello world lf2: %lf", static_cast<double>(100.001));
	LOG(INFO, "hello world p: %p", &s);
	LOG(INFO, "hello world x: %x", 22220);

	/* format */
	LOG(INFO, "hello world d: (%5d)", 100);
	LOG(INFO, "hello world ld: (%2ld)", static_cast<long int>(100));
	LOG(INFO, "hello world u: (%04u)", static_cast<unsigned int>(100));
	LOG(INFO, "hello world lu: (%02lu)", static_cast<unsigned long int>(100));
	LOG(INFO, "hello world f1: (%.2f)", static_cast<float>(100));
	LOG(INFO, "hello world f2: (%.2f)", static_cast<float>(100.001));
	LOG(INFO, "hello world lf1: (%.4lf)", static_cast<double>(100));
	LOG(INFO, "hello world lf2: (%.4lf)", static_cast<double>(100.001));


	/* */
	LOG(INFO, "[%d]hello (%f)world %lf, %.2f, %.5f, %s, %u, %p",
			1,
			static_cast<double>(100.001),
			static_cast<double>(100.001),
			static_cast<double>(100.001),
			static_cast<double>(100.001),
			s,
			static_cast<unsigned int>(1),
			&s
			);

	return ;
}

void testLevel()
{
	LOG(INFO, "= = = = = = = = = = = = = = = = = = = = = = = =");
	LOG(INFO, "= = = = = = = = = = = = = = = = = = = = = = = =");
	LOG(INFO, "= = = = = = = = = = = = = = = = = = = = = = = =");
	vector<Level_E> lvs;
	lvs.push_back(TRACE);
	lvs.push_back(DEBUG);
	lvs.push_back(INFO);
	lvs.push_back(WARN);
	lvs.push_back(ERROR);
	lvs.push_back(CRITICAL);
	lvs.push_back(LOG_OFF);
	for (uint32_t i = 0; i < lvs.size(); ++i)
	{
		Logger::Instance()->setLevel(lvs[i]);
		LOG(CRITICAL, "* * * * * * * * * * * * * * * * * * *");
		LOG(TRACE, "hello wrold. set level(%s)", (LEVEL[lvs[i]]));
		LOG(DEBUG, "hello wrold. set level(%s)", (LEVEL[lvs[i]]));
		LOG(INFO, "hello wrold. set level(%s)", (LEVEL[lvs[i]]));
		LOG(WARN, "hello wrold. set level(%s)", (LEVEL[lvs[i]]));
		LOG(ERROR, "hello wrold. set level(%s)", (LEVEL[lvs[i]]));
		LOG(CRITICAL, "hello wrold. set level(%s)", (LEVEL[lvs[i]]));
		LOG(CRITICAL, "* * * * * * * * * * * * * * * * * * *");
	}

	return ;
}

int main(int argc, char *argv[])
{
	Logger::Instance()->initLog("./dflog.log", loggerOption::FILELOG);

	testDflog();
	testLevel();

	return 0;
}
