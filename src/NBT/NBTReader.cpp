#include "NBTReader.h"
#include "NBTCompound.h"
#include "File/ByteBuffer.h"
#include "File/GzipByteReader.h"
#include "File/WriteBuffer.h"
#include "File/GzipByteWriter.h"
#include "File/FileByteWriter.h"
#include "Exception/NBTException.h"
#include "Exception/FileException.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace NBT {
	NBTCompound* NBTReader::LoadFromFile(const char* filePath) {
		ifstream ifs(filePath, ios::binary | ios::ate);
		if (!ifs)
			throw Exception::FileException(strerror(errno), filePath, errno);

		uint length = ifs.tellg();
		ifs.seekg(0, ios::beg);

		Byte bytes[length];
		ifs.read((char*)bytes, length);
		ifs.close();

		return LoadFromData(bytes, length);
	}

	NBTCompound* NBTReader::LoadFromData(Byte* data, uint length) {
		File::GzipByteReader reader(data, length);
		File::ByteBuffer buffer(&reader);

		return LoadFromUncompressedStream(&buffer);
	}

	NBTCompound* NBTReader::LoadFromUncompressedStream(File::ByteBuffer* buffer) {
		Byte rootType = buffer->ReadByte();
		if (rootType != NbtCompound)
			throw Exception::NBTException("Invalid root tag, must be a compound: " + std::to_string(rootType), NULL);
		buffer->ReadString();

		NBTCompound* rootCompound = new NBTCompound();
		rootCompound->Read(buffer);

		return rootCompound;
	}

	void NBTReader::SaveToFile(const char* filePath, NBTCompound* compound) {
		File::FileByteWriter* fileWriter = new File::FileByteWriter(std::string(filePath));
		File::GzipByteWriter* gzipWriter = new File::GzipByteWriter(fileWriter);

		try {
			File::WriteBuffer buffer(gzipWriter);
			buffer.WriteByte(NbtCompound);
			buffer.WriteString("");

			compound->Write(&buffer);
			gzipWriter->Finish();
		} catch (const Exception::Exception& ex) {
			delete gzipWriter;
			delete fileWriter;
			throw ex;
		}

		delete fileWriter;
		delete gzipWriter;
	}

}