#pragma once
#include "ByteWriter.h"

namespace File {
	class MemoryByteWriter : public ByteWriter {
	public:
		MemoryByteWriter();
		~MemoryByteWriter();

		void WriteByte(const Byte byte);
		void WriteBytes(const Byte* bytes, uint length);
		void Flush();
		
	private:
		Byte* data;
		uint dataLength;
		uint dataOffset;
	};
}