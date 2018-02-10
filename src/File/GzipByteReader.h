#pragma once
#include <zlib.h>
#include "ByteReader.h"

namespace File {
	class GzipByteReader : public ByteReader {
	public:
		GzipByteReader(Byte* data, uint dataLength);
		~GzipByteReader();
		Byte ReadByte();
		Byte* ReadBytes(uint length);

		bool IsFinishedRead() { return finishedRead; }
		uint GetBufferLength() { return bufferLength; }
		void FillNextBuffer();
		Byte* GetBuffer() { return buffer; }

	protected:
//		void FillNextBuffer();

	private:
		z_stream stream;
		bool finishedRead;

		Byte* buffer;
		uint bufferLength;

		uint offset;
		uint bufferOffset;
	};
}