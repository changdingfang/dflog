//***************************************************************
// @file:    fileHelper.h
// @author:  dingfang
// @date    2020-06-12 21:57:59
//***************************************************************

#pragma once

#include <dflog/common.h>
#include <tuple>

namespace dflog
{


    class FileHelper final
    {
    public:
        FileHelper() = default;
        ~FileHelper() {	this->close();	}

        FileHelper(const FileHelper &other) = delete;
        FileHelper(FileHelper &&other) = delete;
        FileHelper operator = (const FileHelper &other) = delete;
        FileHelper operator = (FileHelper &&other) = delete;

        void open(const std::string &filename);
        void reopen();
        void flush();
        void close();
        void write(const formatBuf_t &buf);

        size_t size()
        {
            if (pFd_ == nullptr)
            {
                throwDflogEx("fd is null, filename: " + filename_);
            }
            return dflog::os::filesize(pFd_);
        }

        const std::string &filename() const noexcept { return filename_; }

        static std::tuple<std::string, std::string> splitByExtension(const std::string &filename);

    private:
        std::FILE    *pFd_ = nullptr;
        std::string  filename_ = "";
    };


}; /* namespace dflog end */
