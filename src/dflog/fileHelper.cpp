//***************************************************************
// @file:    fileHelper.cpp
// @author:  dingfang
// @date    2020-06-12 22:15:59
//***************************************************************


#include <dflog/fileHelper.h>


namespace dflog
{


    void FileHelper::open(const std::string &filename)
    {
        this->close();
        filename_ = filename;
        dflog::os::createDir(dflog::os::dirname(filename));

        pFd_ = ::fopen(filename.c_str(), "ab");
        if (pFd_ == nullptr)
        {
            throwDflogEx("open file failed! filename: " + filename, errno);
        }

        return ;
    }


    void FileHelper::reopen()
    {
        if (filename_.empty())
        {
            throwDflogEx("reopen file failed! filename: " + filename_, errno);
        }
        this->open(filename_);
    }


    void FileHelper::flush()
    {
        std::fflush(pFd_);
    }


    void FileHelper::close()
    {
        if (pFd_ != nullptr)
        {
            std::fclose(pFd_);
            pFd_ = nullptr;
        }
        return ;
    }


    void FileHelper::write(const dflog::formatBuf_t &buf)
    {
        size_t msgSize = buf.size();
        const char *data = buf.data();
        if (std::fwrite(data, 1, msgSize, pFd_) != msgSize)
        {
            throwDflogEx("write file failed. filename: " + filename_, errno);
        }

        return ;
    }


    std::tuple<std::string, std::string> FileHelper::splitByExtension(const std::string &filename)
    {
        std::string::size_type pos = filename.rfind('.');
        std::string::size_type folderPos = filename.rfind('/');
        if (pos == std::string::npos || pos == filename.size() - 1 || pos == 0
                || (folderPos != std::string::npos && folderPos >= pos - 1))
        {
            return std::make_tuple(filename, std::string());
        }

        return std::make_tuple(filename.substr(0, pos), filename.substr(pos));
    }


}; /* namespace dflog end */

