#include "FileByteWriter.h"
#include "Exception/FileException.h"
#include <string.h>
#include <errno.h>

using namespace std;

namespace File {
	FileByteWriter::FileByteWriter(std::string filePath) {
		stream.open(filePath, ios::binary | ios::out);
		if (!stream)
			throw Exception::FileException(strerror(errno), filePath.c_str(), errno);
	}

	FileByteWriter::~FileByteWriter() {
		stream.close();
	}

	void FileByteWriter::WriteByte(Byte byte) {
		stream.write((const char*) &byte, 1);
	}

	void FileByteWriter::WriteBytes(const Byte* bytes, uint length) {
		stream.write((const char*) bytes, length);
	}

	void FileByteWriter::Flush() {
		stream.flush();
	}
}