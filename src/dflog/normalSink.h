//***************************************************************
// @file:    normalSink.h
// @author:  dingfang
// @date    2020-06-11 19:52:35
//***************************************************************

#pragma once

#include <dflog/sink.h>
#include <dflog/fileHelper.h>


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
            NormalSink(std::string filename, Rotation_T rotationTime, uint64_t maxFilesize = 1024 * 1024 * 100, uint16_t maxFiles = 10); 
            ~NormalSink() { fileHelper_.close(); }

            virtual void log(const LogMsg_T &msg) final;
            virtual void flush() final;
            virtual void setPattern(const std::string &pattern) final;
            virtual void setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper) final;

            void setFileSize(uint64_t filesize);
            bool setRotationTime(Rotation_T rt);

        private:
            void rotateLog_(time_t time);

        private:
            std::string filename_;
            Rotation_T rotation_;
            uint64_t currentSize_ = 0;
            uint64_t maxFilesize_;
            const uint16_t maxFiles_;
            FileHelper fileHelper_;

            dflog::LogClock_T rotationTime_;
        };


    }; /* namespace sinks end */
}; /* namespace dflog end */
