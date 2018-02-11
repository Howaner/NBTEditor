#pragma once
#include "ByteWriter.h"
#include <zlib.h>

namespace File {
	class GzipByteWriter : public ByteWriter {
	public:
		GzipByteWriter(ByteWriter* parentWriter, bool gzip = true);
		~GzipByteWriter();

		void WriteByte(const Byte byte);

		void WriteBytes(const Byte* bytes, uint length);

		void Flush();
		void Finish();

	protected:
		void DoGzipCompression(int mode, int expectedResult);

	private:
		bool gzip;
		z_stream stream;

		Byte* buffer;
		uint bufferLength;
		uint bufferOffset;

		ByteWriter* parentWriter;
		
	};
}