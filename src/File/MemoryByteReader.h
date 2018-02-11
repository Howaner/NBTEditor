#pragma once
#include "ByteReader.h"

namespace File {
	class MemoryByteReader : public ByteReader {
	public:
		MemoryByteReader(Byte* data, uint dataLength);
		~MemoryByteReader();
		Byte ReadByte();
		Byte* ReadBytes(uint length);

	private:
		Byte* data;
		uint dataLength;
		uint offset;
	};
}