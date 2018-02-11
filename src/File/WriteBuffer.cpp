#include "WriteBuffer.h"
#include <sstream>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace File {
	WriteBuffer::WriteBuffer(ByteWriter* writer) : writer(writer) {}

	void WriteBuffer::WriteByte(const Byte byte) {
		writer->WriteByte(byte);
	}

	void WriteBuffer::WriteBytes(const Byte* bytes, uint length) {
		writer->WriteBytes(bytes, length);
	}

	void WriteBuffer::WriteSignedByte(jbyte val) {
		WriteByte((Byte) val);
	}

	void WriteBuffer::WriteShort(jshort val) {
		WriteByte((val >> 8) & 0xFF);
		WriteByte(val & 0xFF);
	}

	void WriteBuffer::WriteThreeBytesInt(jint val) {
		WriteByte((val >> 16) & 0x0F);
		WriteByte((val >> 8) & 0xFF);
		WriteByte(val & 0xFF);
	}

	void WriteBuffer::WriteInt(jint val) {
		Byte bytes[4];
		memcpy(bytes, &val, 4);

		std::reverse(bytes, bytes + 4);  // Little-Endian to Big-Endian
		WriteBytes(bytes, 4);
	}

	void WriteBuffer::WriteLong(jlong val) {
		Byte bytes[8];
		memcpy(bytes, &val, 8);

		std::reverse(bytes, bytes + 8);  // Little-Endian to Big-Endian
		WriteBytes(bytes, 8);
	}

	void WriteBuffer::WriteDouble(jdouble val) {
		Byte bytes[8];
		memcpy(bytes, &val, 8);

		std::reverse(bytes, bytes + 8);  // Little-Endian to Big-Endian
		WriteBytes(bytes, 8);
	}

	void WriteBuffer::WriteFloat(jfloat val) {
		Byte bytes[4];
		memcpy(bytes, &val, 4);

		std::reverse(bytes, bytes + 4);  // Little-Endian to Big-Endian
		WriteBytes(bytes, 4);
	}

	void WriteBuffer::WriteString(QString val) {
		QByteArray byteArray(val.toUtf8());

		WriteShort(byteArray.length());
		WriteBytes(reinterpret_cast<const Byte*>(byteArray.constData()), byteArray.length());
	}

}
