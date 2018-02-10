#pragma once
#include "Exception.h"

namespace Exception {

	class StreamOverflowException : public Exception {
	public:
		StreamOverflowException(uint offset, uint bytesToRead, uint capacity)
			: Exception("Can't read " + std::to_string(bytesToRead) + " bytes on index " + std::to_string(offset) + " because that would increase stream capacity of " + std::to_string(capacity) + " bytes")
			{
			}
	};
}