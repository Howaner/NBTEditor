#include "NBTEntry.h"
#include "NBTTag.h"
#include "NBTHelper.h"

namespace NBT {
	NBTEntry::~NBTEntry() {
		name.clear();
		type = NbtEnd;

		if (value != NULL) {
			const NBTTag* tag = NBTHelper::GetTagByType(type);
			if (tag != NULL) {
				tag->Free(*this);
			}
		}
	}
}