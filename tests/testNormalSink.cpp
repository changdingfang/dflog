//***************************************************************
// @file:    testNormalSink.cpp
// @author:  dingfang
// @date    2020-06-16 20:23:41
//***************************************************************

#include <dflog/normalSink.h>
#include <gtest/gtest.h>

#include <sys/time.h>

#include <memory>

using namespace std;
using namespace dflog;
using namespace sinks;
using namespace fmtHelper;
using namespace level;

bool fileExists(string filename)
{
    return (!::access(filename.c_str(), F_OK));
}

string rotationFileName(string filename, time_t sec)
{
    char timeStr[32] = { 0 };
    struct tm tm;
    localtime_r(&sec,  &tm);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d", &tm);
    string ext, f;
    std::tie(f, ext) = FileHelper::splitByExtension(filename);

    return std::move(f.append("_").append(timeStr).append(ext));
}

int testNormalSink(string filename, Rotation_T rt, uint64_t maxFilesize, uint16_t maxFiles, LogMsg_T msg)
{
    shared_ptr<NormalSink> p = make_shared<NormalSink>(filename,  rt, maxFilesize, maxFiles);
    p->setFormatter(unique_ptr<FormatHelper>(new FormatHelper()));

    if (!fileExists(filename))
    {
        return 1;
    }

    for (int i = 0; i < 10; ++i)
    {
        p->log(msg);
    }

    return 0;
}


TEST(NormalSink, rotateLog)
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    struct tm tm;
    localtime_r(&tv.tv_sec, &tm);

    SrcLoc_T sl(__FILE__, __LINE__, __FUNCTION__);

    LogMsg_T msg(LogClock_T(tv), sl, "dflog", INFO, "hello world");

    /* 测试日志大小分割和日志格式支持 */
    vector< pair<string, uint64_t> > tests {
        { "test1.log", 1 << 20 },
            { "test2.log", 256 },
            { "./test3.log" ,1 << 20 },
            { "./log/test4.log" ,1 << 20 },
            { "./log/test5.txt" ,1 << 20 },
            { "./log/test6" ,1 << 20 },
            { "./log/test7.log" ,256 },
            { "./log/test8" ,256 },
            { "./log/test9.x" ,512 },
            { "/tmp/log/test10..b" ,512 },
            { "/tmp/.dflog/.log/.test11.log" ,128 },
    };

    for (uint32_t i = 0; i < tests.size(); ++i)
    {
        EXPECT_FALSE(testNormalSink(tests[i].first, Rotation_T{6, 30}, tests[i].second, 10, msg));
        if (tests[i].second <= 512)
        {
            string rotationFile = rotationFileName(tests[i].first, msg.time.sec);
            EXPECT_TRUE(fileExists(rotationFile)) << "[name: " << rotationFile << "]";
        }

        /* 测试每日分割日志 */
        msg.time.sec += ONE_DAY_SEC;
        EXPECT_FALSE(testNormalSink(tests[i].first, Rotation_T{6, 30}, tests[i].second, 10, msg));
        string rotationFile(rotationFileName(tests[i].first, msg.time.sec));
        EXPECT_TRUE(fileExists(tests[i].first));
        EXPECT_TRUE(fileExists(rotationFile)) << "rotationFile: " << rotationFile;
        msg.time.sec -= ONE_DAY_SEC;
    }

}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
