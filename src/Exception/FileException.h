#pragma once
#include "Exception.h"

namespace Exception {
	class FileException : public Exception {
	public:
		FileException(const char* error, const char* filePath, int ioErrorCode)
			: Exception("Exception while reading file " + std::string(filePath) + ": " + std::string(error) + ", code " + std::to_string(ioErrorCode))
			{
			}
		
	private:
		
	};
}