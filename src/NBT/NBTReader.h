#include "NBTCompound.h"

namespace NBT {
	class NBTReader {
	public:
		static NBTCompound* LoadFromFile(const char* filePath, NBTFileType* outType = NULL);
		static NBTCompound* LoadFromData(Byte* data, uint length, NBTFileType* outType = NULL);
		static NBTCompound* LoadFromUncompressedStream(File::ByteBuffer* buffer);

		static void SaveToFile(const char* filePath, NBTCompound* compound);
		static void SaveToFileUncompressed(const char* filePath, NBTCompound* compound);
	};
}