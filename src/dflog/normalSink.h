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
			T_Rotation() = default;
			T_Rotation(int rh, int rm)
				: rotationH(rh)
				, rotationM(rm)
			{	}
			bool operator > (const T_Rotation &other) const
			{
				if (this->rotationH > other.rotationH || 
					(this->rotationH == other.rotationH && this->rotationM > other.rotationM))
				{
					return true;
				}
				return false;
			}

			int rotationH;
			int rotationM;
		}Rotation_T;


		class NormalSink : public Sink
		{
		public:
			NormalSink(std::string filename, Rotation_T rotationTime, uint64_t maxFilesize = 1024 * 1024 * 100, uint16_t maxFiles = 10); 
			virtual ~NormalSink()
			{	fileHelper_.close();	}

			NormalSink(const NormalSink &other) = delete;
			NormalSink(NormalSink &&other) = delete;

			NormalSink &operator = (const NormalSink &other) = delete;
			NormalSink &operator = (NormalSink &&other) = delete;

			virtual void log(const LogMsg_T &msg) final;
			virtual void flush() final;
			virtual void setPattern(const std::string &pattern) final;
			virtual void setFormatter(std::unique_ptr<dflog::fmtHelper::FormatHelper> sinkFormatHelper) final;

		private:
			void rotateLog_(time_t time);
		private:
			std::string filename_;
			Rotation_T rotation_;
			uint64_t currentSize_ = 0;
			const uint64_t maxFilesize_;
			const uint16_t maxFiles_;
			FileHelper fileHelper_;

			dflog::LogClock_T rotationTime_;
		};

	}; /* namespace sinks end */
}; /* namespace dflog end */
