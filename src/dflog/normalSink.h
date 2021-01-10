//***************************************************************
// @file:    normalSink.h
// @author:  dingfang
// @date    2020-06-11 19:52:35
//***************************************************************

#pragma once

#include <dflog/sink.h>
#include <dflog/fileHelper.h>

#include <list>

namespace dflog
{

    namespace sinks
    {
        typedef struct T_Rotation
        {
            int rotationH;
            int rotationM;
        }Rotation_T;


        class NormalSink final : public Sink
        {
        public:
            NormalSink(std::string filename, Rotation_T rotationTime, uint64_t maxFilesize = 1024 * 1024 * 100, int16_t maxFiles = -1); 
            ~NormalSink() { fileHelper_.close(); }

            virtual void log(const LogMsg_T &msg) final;
            virtual void flush() final;
            virtual void setPattern(const std::string &pattern) final;
            virtual void setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper) final;

            void setFileSize(uint64_t filesize);
            bool setRotationTime(Rotation_T rt);
            void setFiles(int16_t maxFiles);

        private:
            void rotateLog(time_t time);

            void loadLogFilename();
            void cleanOldFile();

        private:
            std::string filename_;
            Rotation_T rotation_;
            uint64_t currentSize_ = 0;
            uint64_t maxFilesize_;
            int16_t maxFiles_;
            FileHelper fileHelper_;

            std::list< std::list<std::string> > filenameList_;
            int16_t currFiles_;

            dflog::LogClock_T rotationTime_;
        };


    }; /* namespace sinks end */
}; /* namespace dflog end */
