#include "MemoryByteReader.h"
#include "Exception/GzipException.h"
#include <cstring>
#include "Exception/StreamOverflowException.h"
#include <string>
#include <iostream>

namespace File {
	MemoryByteReader::MemoryByteReader(Byte* data, uint dataLength) : data(data), dataLength(dataLength), offset(0) {
	}

	MemoryByteReader::~MemoryByteReader() {
		data = NULL;
		dataLength = 0;
		offset = 0;
	}

	Byte MemoryByteReader::ReadByte() {
		if (offset >= dataLength) {
			throw Exception::StreamOverflowException(offset, 1, dataLength);
		}

		return data[offset++];
	}

	Byte* MemoryByteReader::ReadBytes(uint length) {
		if (offset + length >= dataLength)
			throw Exception::StreamOverflowException(offset, length, dataLength);

		Byte* readBytes = new Byte[length];
		memcpy(readBytes, data + offset, length);

		offset += length;
		return readBytes;
	}
}