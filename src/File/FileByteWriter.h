#pragma once
#include "ByteWriter.h"
#include <iostream>
#include <fstream>

namespace File {
	class FileByteWriter : public ByteWriter {
	public:
		FileByteWriter(std::string filePath);
		~FileByteWriter();

		void WriteByte(const Byte byte);

		void WriteBytes(const Byte* bytes, uint length);

		void Flush();
		
	private:
		std::ofstream stream;
	};
}