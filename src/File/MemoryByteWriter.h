#pragma once
#include "Globals.h"
#include "ByteWriter.h"
#include "NBT/NBTEntry.h"

namespace File {
	class MemoryByteWriter : public ByteWriter {
	public:
		MemoryByteWriter();
		~MemoryByteWriter();

		void WriteByte(const Byte byte);
		void WriteBytes(const Byte* bytes, uint length);
		void Flush() {}

		NBT::NBTArray<Byte> GetByteArray();
		uint GetOffset() { return bufferOffset; }

		void SetBufferByte(uint offset, const Byte byte);

	protected:
		void IncreaseBufferSize();
		
	private:
		Byte* buffer;
		uint bufferOffset;
		uint bufferSize;
	};
}