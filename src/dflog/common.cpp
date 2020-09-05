//***************************************************************
// @file:    common.cpp
// @author:  dingfang
// @date    2020-06-13 09:00:01
//***************************************************************

#include <dflog/common.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>


namespace dflog
{

    DflogEx::DflogEx(std::string msg)
        : msg_(std::move(msg))
    { }

    DflogEx::DflogEx(const std::string &msg, int lastErrno)
    {
        msg_ = msg + ": " + std::to_string(lastErrno);
    }

    const char *DflogEx::what() const noexcept 
    {
        return msg_.c_str();
    }

    void throwDflogEx(const std::string &msg, int lastErrno)
    {
        throw(DflogEx(msg, lastErrno));
    }

    void throwDflogEx(std::string msg)
    {
        throw(DflogEx(std::move(msg)));
    }


    namespace os
    {


        static bool mkdir_(const std::string &path)
        {
            return (::mkdir(path.c_str(), 0755) == 0);
        }

        bool createDir(std::string path)
        {
            if (path.empty())
            {
                return false;
            }

            if (pathExists(path))
            {
                return true;
            }

            size_t searchOffset = 0;
            do
            {
                std::string::size_type pos = path.find('/', searchOffset);
                if (pos == std::string::npos)
                {
                    pos = path.size();
                }
                std::string subdir = path.substr(0, pos);
                if (!subdir.empty() && !pathExists(subdir) && !mkdir_(subdir))
                {
                    return false;
                }
                searchOffset = pos + 1;
            } while (searchOffset < path.size());

            return true;
        }


    }; /* namespace os end */

}; /* namespace common end */
