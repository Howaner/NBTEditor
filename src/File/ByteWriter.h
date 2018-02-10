#pragma once
#include "Globals.h"

namespace File {
	class ByteWriter {
	public:
		virtual ~ByteWriter() {};

		/* Writes one byte to the stream */
		virtual void WriteByte(const Byte byte) = 0;

		/* Writes the specific amount of bytes to the stream. */
		virtual void WriteBytes(const Byte* bytes, uint length) = 0;

		/* Writes all remaining bytes in the buffer to the stream. */
		virtual void Flush() = 0;
	};
}