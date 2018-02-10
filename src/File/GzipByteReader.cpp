#include "GzipByteReader.h"
#include "Exception/GzipException.h"
#include <cstring>
#include "Exception/StreamOverflowException.h"
#include <string>
#include <iostream>
#include <algorithm>

namespace File {
	GzipByteReader::GzipByteReader(Byte* data, uint dataLength) : finishedRead(false), bufferLength(0), offset(0), bufferOffset(0) {
		memset(&stream, 0, sizeof stream);
		stream.zalloc = (alloc_func)0;
		stream.zfree = (free_func)0;
		stream.opaque = (voidpf)0;

		stream.next_in = data;
		stream.avail_in = dataLength;

		buffer = new Byte[GZIP_BUFFER_SIZE];

		int result = inflateInit2(&stream, MAX_WBITS | 16);
		if (result != 0)
			throw Exception::GzipException("ZLIB init failed: " + std::to_string(result));
		FillNextBuffer();
	}

	GzipByteReader::~GzipByteReader() {
		inflateEnd(&stream);

		delete[] buffer;
		bufferLength = 0;
		offset = 0;
		bufferOffset = 0;
	}

	void GzipByteReader::FillNextBuffer() {
		if (finishedRead)
			throw Exception::GzipException("Read already finished. There is no more data to read.");

		stream.next_out = buffer;
		stream.avail_out = GZIP_BUFFER_SIZE;

		int result = inflate(&stream, Z_NO_FLUSH);
		switch (result) {
			case Z_STREAM_END:
				finishedRead = true;
				bufferLength = (uint)stream.total_out - offset;
				bufferOffset = 0;
				break;
			case Z_OK:
				bufferLength = (uint)stream.total_out - offset;
				bufferOffset = 0;
				break;
			default:
				throw Exception::GzipException("ZLIB read failed: " + std::to_string(result));
				break;
		}
	}

	Byte GzipByteReader::ReadByte() {
		if (bufferOffset >= bufferLength) {
			if (finishedRead)
				throw Exception::StreamOverflowException(offset, 1, offset);

			FillNextBuffer();
		}

		offset++;
		return buffer[bufferOffset++];
	}

	Byte* GzipByteReader::ReadBytes(uint length) {
		Byte* readBytes = new Byte[length];
		uint readOffset = 0;
		uint startOffset = offset;

		while (readOffset < length) {
			int dataLength = std::min(bufferLength - bufferOffset, length - readOffset);
			if (dataLength == 0) {
				if (finishedRead)
					throw Exception::StreamOverflowException(startOffset, length, offset);
				FillNextBuffer();
			}

			memcpy(readBytes + readOffset, buffer + bufferOffset, dataLength);
			readOffset += dataLength;
			bufferOffset += dataLength;
			offset += dataLength;
		}

		return readBytes;
	}
}