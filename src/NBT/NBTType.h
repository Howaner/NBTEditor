#pragma once

namespace NBT {
	enum NBTType {
		NbtEnd = 0,
		NbtByte = 1,
		NbtShort = 2,
		NbtInt = 3,
		NbtLong = 4,
		NbtFloat = 5,
		NbtDouble = 6,
		NbtByteArray = 7,
		NbtString = 8,
		NbtList = 9,
		NbtCompound = 10,
		NbtIntArray = 11,
		NbtLongArray = 12
	};

	enum NBTFileType {
		NbtGzipCompressed = 0,
		NbtUncompressed = 1,
		NbtAnvilRegion = 2
	};
}