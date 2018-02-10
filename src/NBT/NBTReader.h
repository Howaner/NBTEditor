#include "NBTCompound.h"

namespace NBT {
	class NBTReader {
	public:
		static NBTCompound* LoadFromFile(const char* filePath);
		static NBTCompound* LoadFromData(Byte* data, uint length);
		static NBTCompound* LoadFromUncompressedStream(File::ByteBuffer* buffer);

		static void SaveToFile(const char* filePath, NBTCompound* compound);
	};
}