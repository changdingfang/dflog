//***************************************************************
// @file:    testFMT.cpp
// @author:  dingfang
// @date    2020-06-18 21:13:58
//***************************************************************

#include <iostream>
#include <string>
#include <dflog/dflog.h>
#include <fmt/color.h>
#include <sys/time.h>
#include <stack>
#include <stdio.h>

using namespace std;
using namespace fmt;

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


void test(int32_t lines, int method)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    string str;
    char strc[1024] = {0};
    switch (method)
    {
        case 1:
            {
                for (int i = 0; i < lines; ++i)
                {
                    fmt::format("hello world{}, {}", i, (double)i);
                }
                printf("method:\t\t(fmt)\n");
                break;
            }
        case 2:
            {
                for (int i = 0; i < lines; ++i)
                {
                    sprintf(strc, "hello world%d, %lf", i, (double)i);
                }
                printf("method:\t\t(sprintf)\n");
                break;
            }
    }
    struct timeval tv2;
    gettimeofday(&tv2, nullptr);

    double use = ((tv2.tv_sec - tv.tv_sec) * ONE_MILLION  + (tv2.tv_usec - tv.tv_usec)) * 1.0 / ONE_MILLION;
    printf("lines:\t\t(%s)\n", commaNum(lines).c_str());
    printf("use time:\t%.3lf (s)\n", use);
    printf("speed:\t\t%s (line/s)\n", commaNum(static_cast<uint32_t>(lines / use)).c_str());
    printf("speed:\t\t%.3lf (us/line)\n", (use * ONE_MILLION) / lines);
    printf("* * * * * * * * * * * * * * * * * * *\n");

    return ;
}


int main(void)
{
    constexpr uint32_t lines =  10 * 10000;
    test(lines, 1);
    test(lines, 2);

    return 0;
}
