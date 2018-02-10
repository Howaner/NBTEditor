#pragma once
#include "Exception.h"

namespace NBT {
	class NBTEntry;
}

namespace Exception {
	class NBTException : public Exception {
	public:
		explicit NBTException(const char* message, NBT::NBTEntry* entry): Exception(message), entry(entry)
		{
		}

		explicit NBTException(const std::string& message, NBT::NBTEntry* entry): Exception(message), entry(entry)
		{
		}
		
	private:
		NBT::NBTEntry* entry;
	};
}