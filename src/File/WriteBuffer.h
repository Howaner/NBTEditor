#pragma once
#include "Globals.h"
#include "ByteWriter.h"
#include <QString>

namespace File {
	
	class WriteBuffer {
	public:
		WriteBuffer(ByteWriter* writer);

		void WriteByte(const Byte byte);
		void WriteBytes(const Byte* bytes, uint length);

		void WriteSignedByte(jbyte val);
		void WriteShort(jshort val);
		void WriteInt(jint val);
		void WriteLong(jlong val);
		void WriteDouble(jdouble val);
		void WriteFloat(jfloat val);
		void WriteString(QString val);
		
	private:
		ByteWriter* writer;
	};
}