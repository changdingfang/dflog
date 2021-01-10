// =======================================================================
//  FileName:     testLogLoop.cpp
//  Author:       dingfang
//  CreateDate:   2021-01-10 17:44:10
//  ModifyAuthor: dingfang
//  ModifyDate:   2021-01-10 18:40:28
// =======================================================================

/* 
 * 测试日志文件数量限制功能 
 * */

#include <dflog/dflog.h>
#include <iostream>
#include <string>

using namespace std;
using namespace dflog;


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("please input file nums\n");
        return -1;
    }
    dflog::InitLog("./dflog_loop/dflog_loop.log", loggerOption::FILELOG);

    dflog::SetFiles(stoi(argv[1]));

    return 0;
}
