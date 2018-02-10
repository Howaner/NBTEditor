#pragma once
#include "Exception.h"

namespace Exception {
	class GzipException : public Exception {
	public:
		explicit GzipException(const char* message): Exception(message)
		{
		}

		explicit GzipException(const std::string& message): Exception(message)
		{
		}
	};
}