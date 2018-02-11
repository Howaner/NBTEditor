#pragma once
#include "NBTEntry.h"
#include "Globals.h"
#include <vector>

namespace File {
	class ByteBuffer;
	class WriteBuffer;
}

namespace NBT {
	class NBTTag;

	class NBTCompound {
	public:
		NBTCompound();
		NBTCompound(NBTCompound* oldCompound);  // Create a copy
		~NBTCompound();

		void Free();

		void Read(File::ByteBuffer* buffer);
		void Write(File::WriteBuffer* buffer);

		NBTEntry* FindName(QString name);

		const std::vector<NBTEntry*>& GetEntries();
		void AddEntry(NBTEntry* entry, int position = -1);
		bool RemoveEntry(NBTEntry* entry);

	private:
		bool noFree;
		std::vector<NBTEntry*> entries;
	};
}