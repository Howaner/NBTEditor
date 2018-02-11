#include "GzipByteWriter.h"
#include "Exception/GzipException.h"
#include "GzipByteReader.h"
#include <cstring>
#include <string>
#include <algorithm>

namespace File {

	GzipByteWriter::GzipByteWriter(ByteWriter* parentWriter, bool gzip) : gzip(gzip), bufferLength(0), bufferOffset(0), parentWriter(parentWriter) {
		memset(&stream, 0, sizeof stream);
		stream.zalloc = (alloc_func)0;
		stream.zfree = (free_func)0;
		stream.opaque = (voidpf)0;

		buffer = new Byte[GZIP_BUFFER_SIZE];
		bufferLength = GZIP_BUFFER_SIZE;

		stream.avail_in = bufferOffset;
		stream.next_in = buffer;

		int zlibFlags = (gzip ? (MAX_WBITS | 16) : MAX_WBITS);
		int result = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, zlibFlags, 8, Z_DEFAULT_STRATEGY);
		if (result != 0)
			throw Exception::GzipException("ZLIB deflate init failed: " + std::to_string(result));
	}

	GzipByteWriter::~GzipByteWriter() {
		deflateEnd(&stream);
		delete[] buffer;

		bufferLength = 0;
		bufferOffset = 0;
	}

	void GzipByteWriter::WriteByte(const Byte byte) {
		if (bufferOffset >= bufferLength)
			Flush();

		buffer[bufferOffset] = byte;
		bufferOffset++;
	}

	void GzipByteWriter::WriteBytes(const Byte* bytes, uint length) {
		uint writtenBytes = 0;
		while (writtenBytes < length) {
			if (bufferOffset >= bufferLength)
				Flush();

			int dataLength = std::min(bufferLength - bufferOffset, length - writtenBytes);
			memcpy(buffer + bufferOffset, bytes + writtenBytes, dataLength);
			bufferOffset += dataLength;
			writtenBytes += dataLength;
		}
	}

	void GzipByteWriter::Flush() {
		if (bufferOffset == 0)
			return;

		DoGzipCompression(Z_SYNC_FLUSH, Z_OK);
	}

	void GzipByteWriter::Finish() {
		DoGzipCompression(Z_FINISH, Z_STREAM_END);
		parentWriter->Flush();
	}

	void GzipByteWriter::DoGzipCompression(int mode, int expectedResult) {
		stream.avail_in = bufferOffset;
		stream.next_in = buffer;

		Byte writeBuffer[GZIP_BUFFER_SIZE];

		int result;
		do {
			uLong oldTotalOut = stream.total_out;
			stream.avail_out = GZIP_BUFFER_SIZE;
			stream.next_out = writeBuffer;

			result = deflate(&stream, mode);
			parentWriter->WriteBytes(writeBuffer, (uint) (stream.total_out - oldTotalOut));
		} while (stream.avail_out == 0);

		if (result != expectedResult)
			throw Exception::GzipException("ZLIB compression failed: " + std::to_string(result) + ", expected result: " + std::to_string(expectedResult));

		bufferOffset = 0;
	}

}