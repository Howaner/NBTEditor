#include "ByteBuffer.h"
#include <sstream>
#include <cstring>
#include <algorithm>
#include <QString>

namespace File {
	ByteBuffer::ByteBuffer(ByteReader* reader) : reader(reader) {}

	Byte ByteBuffer::ReadByte() {
		return reader->ReadByte();
	}

	Byte* ByteBuffer::ReadBytes(int length) {
		return reader->ReadBytes(length);
	}

	jbyte ByteBuffer::ReadSignedByte() {
		return (jbyte) ReadByte();
	}

	jshort ByteBuffer::ReadShort() {
		return (((short) reader->ReadByte()) << 8) | reader->ReadByte();
	}

	unsigned short ByteBuffer::ReadUShort() {
		return (((unsigned short) reader->ReadByte()) << 8) | reader->ReadByte();
	}

	jint ByteBuffer::ReadThreeBytesInt() {
		Byte* bytes = ReadBytes(3);
		jint number = ((bytes[0] & 0x0F) << 16) | ((bytes[1] & 0xFF) << 8) | ((bytes[2] & 0xFF));

		delete[] bytes;
		return number;
	}

	jint ByteBuffer::ReadInt() {
		Byte* bytes = ReadBytes(4);
		std::reverse(bytes, bytes + 4);  // Big-Endian to Little-Endian

		jint number;
		memcpy(&number, bytes, 4);
		delete[] bytes;
		return number;
	}

	jlong ByteBuffer::ReadLong() {
		Byte* bytes = ReadBytes(8);
		std::reverse(bytes, bytes + 8);  // Big-Endian to Little-Endian

		jlong number;
		memcpy(&number, bytes, 8);
		delete[] bytes;
		return number;
	}

	jdouble ByteBuffer::ReadDouble() {
		Byte* bytes = ReadBytes(8);
		uint64_t longNumber =
			uint64_t(bytes[7])
			| (uint64_t(bytes[6]) << 8)
			| (uint64_t(bytes[5]) << 16)
			| (uint64_t(bytes[4]) << 24)
			| (uint64_t(bytes[3]) << 32)
			| (uint64_t(bytes[2]) << 40)
			| (uint64_t(bytes[1]) << 48)
			| (uint64_t(bytes[0]) << 56);
		delete[] bytes;

		jdouble number = 0;
		memcpy(&number, &longNumber, 8);
		return number;
	}

	jfloat ByteBuffer::ReadFloat() {
		Byte* bytes = ReadBytes(4);
		uint32_t intNumber =
			uint32_t(bytes[3])
			| (uint32_t(bytes[2]) << 8)
			| (uint32_t(bytes[1]) << 16)
			| (uint32_t(bytes[0]) << 24);
		delete[] bytes;

		jfloat number = 0;
		memcpy(&number, &intNumber, 4);
		return number;
	}

	QString ByteBuffer::ReadString() {
		// Read simple utf-8 bytes and convert it to QString. UTF-8 is the same as described in https://docs.oracle.com/javase/7/docs/api/java/io/DataInput.html#readUTF()

		unsigned short length = ReadUShort();
		char* chars = reinterpret_cast<char*>(ReadBytes(length));

		return QString::fromUtf8(chars, length);
	}
}