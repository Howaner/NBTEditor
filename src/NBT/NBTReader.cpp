#include "NBTReader.h"
#include "NBTCompound.h"
#include "File/ByteBuffer.h"
#include "File/GzipByteReader.h"
#include "File/MemoryByteReader.h"
#include "File/WriteBuffer.h"
#include "File/GzipByteWriter.h"
#include "File/FileByteWriter.h"
#include "Exception/NBTException.h"
#include "Exception/FileException.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace NBT {
	NBTCompound* NBTReader::LoadFromFile(const char* filePath, NBTFileType* outType) {
		ifstream ifs(filePath, ios::binary | ios::ate);
		if (!ifs)
			throw Exception::FileException(strerror(errno), filePath, errno);

		uint length = ifs.tellg();
		ifs.seekg(0, ios::beg);

		Byte bytes[length];
		ifs.read((char*)bytes, length);
		ifs.close();

		return LoadFromData(bytes, length, outType);
	}

	NBTCompound* NBTReader::LoadFromData(Byte* data, uint length, NBTFileType* outType) {
		if (length > 1 && data[0] == NbtCompound) {
			// Maybe the file is uncompressed - try to load it without gzip compression
			std::cout << "Try to load the nbt file without compression." << std::endl;

			File::MemoryByteReader reader(data, length);
			File::ByteBuffer buffer(&reader);

			try {
				NBTCompound* compound = LoadFromUncompressedStream(&buffer);
				if (outType != NULL)
					*outType = NbtUncompressed;
				return compound;
			} catch (const Exception::Exception& ex) {
				std::cout << "Loading without compression failed: " << ex.GetMessage().c_str() << std::endl;
			}
		}

		File::GzipByteReader reader(data, length);
		File::ByteBuffer buffer(&reader);

		if (outType != NULL)
			*outType = NbtGzipCompressed;

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

	void NBTReader::SaveToFileUncompressed(const char* filePath, NBTCompound* compound) {
		File::FileByteWriter* fileWriter = new File::FileByteWriter(std::string(filePath));

		try {
			File::WriteBuffer buffer(fileWriter);
			buffer.WriteByte(NbtCompound);
			buffer.WriteString("");

			compound->Write(&buffer);
		} catch (const Exception::Exception& ex) {
			delete fileWriter;
			throw ex;
		}

		delete fileWriter;
	}

}