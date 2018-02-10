#pragma once
#include "Globals.h"
#include "ByteReader.h"
#include <QString>

namespace File {

	class ByteBuffer {
	public:
		ByteBuffer(ByteReader* reader);

		Byte ReadByte();
		Byte* ReadBytes(int length);

		jbyte ReadSignedByte();
		jshort ReadShort();
		unsigned short ReadUShort();
		jint ReadInt();
		jlong ReadLong();
		jdouble ReadDouble();
		jfloat ReadFloat();
		QString ReadString();

	private:
		ByteReader* reader;

	};
}