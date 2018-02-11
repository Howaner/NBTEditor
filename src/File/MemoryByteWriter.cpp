#include "MemoryByteWriter.h"
#include "Exception/StreamOverflowException.h"
#include <cstring>
#include <algorithm>

namespace File {

	MemoryByteWriter::MemoryByteWriter() : bufferOffset(0) {
		buffer = new Byte[WRITE_BUFFER_SIZE];
		bufferSize = WRITE_BUFFER_SIZE;
	}

	MemoryByteWriter::~MemoryByteWriter() {
		delete[] buffer;
		bufferOffset = 0;
		bufferSize = 0;
	}

	void MemoryByteWriter::WriteByte(const Byte byte) {
		if (bufferOffset >= bufferSize)
			IncreaseBufferSize();

		buffer[bufferOffset++] = byte;
	}

	void MemoryByteWriter::WriteBytes(const Byte* bytes, uint length) {
		uint writtenBytes = 0;
		while (writtenBytes < length) {
			if (bufferOffset >= bufferSize)
				IncreaseBufferSize();

			uint bytesToWrite = std::min(bufferSize - bufferOffset, length - writtenBytes);
			std::memcpy(buffer + bufferOffset, bytes + writtenBytes, bytesToWrite);

			bufferOffset += bytesToWrite;
			writtenBytes += bytesToWrite;
		}
	}

	NBT::NBTArray<Byte> MemoryByteWriter::GetByteArray() {
		Byte* resultArray = new Byte[bufferOffset];
		std::memcpy(resultArray, buffer, bufferOffset);

		NBT::NBTArray<Byte> returnArray((int)bufferOffset, resultArray);
		return returnArray;
	}

	void MemoryByteWriter::SetBufferByte(uint offset, const Byte byte) {
		if (offset >= bufferSize)
			throw Exception::StreamOverflowException(offset, 1, bufferSize);
		buffer[offset] = byte;
	}

	void MemoryByteWriter::IncreaseBufferSize() {
		int newSize = bufferSize * 2;
		Byte* newBuffer = new Byte[newSize];
		std::memcpy(newBuffer, buffer, bufferSize);

		buffer = newBuffer;
		bufferSize = newSize;
	}
}