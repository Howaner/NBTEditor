#pragma once
#include "Globals.h"

namespace File {
	class ByteReader {
	public:
		virtual ~ByteReader() {};

		/* Reads one byte from the stream */
		virtual Byte ReadByte() = 0;

		/* Reads the specific amount of bytes from the stream. If the stream has not enough data to read the full amount, the method will return false. */
		virtual Byte* ReadBytes(uint length) = 0;
	};
}