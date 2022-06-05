#include "NBTHelper.h"
#include "NBTTag.h"
#include <algorithm>

namespace NBT {

	const NBTTag** NBTHelper::tagsByType = [] {
		const NBTTag** tags = new const NBTTag*[MAX_TAG + 1];
		std::fill(tags, tags + MAX_TAG, (const NBTTag*)NULL);
		tags[NbtByte] = new NBTTagByte();
		tags[NbtShort] = new NBTTagShort();
		tags[NbtInt] = new NBTTagInt();
		tags[NbtLong] = new NBTTagLong();
		tags[NbtFloat] = new NBTTagFloat();
		tags[NbtDouble] = new NBTTagDouble();
		tags[NbtByteArray] = new NBTTagByteArray();
		tags[NbtString] = new NBTTagString();
		tags[NbtList] = new NBTTagList();
		tags[NbtCompound] = new NBTTagCompound();
		tags[NbtIntArray] = new NBTTagIntArray();
		tags[NbtLongArray] = new NBTTagLongArray();
		return tags;
	}();

	const NBTTag* NBTHelper::GetTagByType(NBTType type) {
		if (type < MIN_TAG || type > MAX_TAG)
			return NULL;

		return tagsByType[type];
	}

	template<typename T> const T* NBTHelper::GetTag(NBTType type) {
		const NBTTag* tag = GetTagByType(type);
		if (tag == NULL)
			return NULL;

		return static_cast<const T*>(tag);
	}

	template const NBTTagByteArray* NBTHelper::GetTag<NBTTagByteArray>(NBTType type);
	template const NBTTagIntArray* NBTHelper::GetTag<NBTTagIntArray>(NBTType type);

	const NBTTag** NBTHelper::GetAllTags() {
		return tagsByType;
	}

	jbyte NBTHelper::GetByte(NBTEntry& entry) {
		return GetTag<NBTTagByte>(NbtByte)->GetData(entry);
	}
	jshort NBTHelper::GetShort(NBTEntry& entry) {
		return GetTag<NBTTagShort>(NbtShort)->GetData(entry);
	}
	jint NBTHelper::GetInt(NBTEntry& entry) {
		return GetTag<NBTTagInt>(NbtInt)->GetData(entry);
	}
	jlong NBTHelper::GetLong(NBTEntry& entry) {
		return GetTag<NBTTagLong>(NbtLong)->GetData(entry);
	}
	jfloat NBTHelper::GetFloat(NBTEntry& entry) {
		return GetTag<NBTTagFloat>(NbtFloat)->GetData(entry);
	}
	jdouble NBTHelper::GetDouble(NBTEntry& entry) {
		return GetTag<NBTTagDouble>(NbtDouble)->GetData(entry);
	}
	NBTArray<jbyte>& NBTHelper::GetByteArray(NBTEntry& entry) {
		return GetTag<NBTTagByteArray>(NbtByteArray)->GetData(entry);
	}
	QString& NBTHelper::GetString(NBTEntry& entry) {
		return GetTag<NBTTagString>(NbtString)->GetData(entry);
	}
	NBTList& NBTHelper::GetList(NBTEntry& entry) {
		return GetTag<NBTTagList>(NbtList)->GetData(entry);
	}
	NBTCompound* NBTHelper::GetCompound(NBTEntry& entry) {
		return GetTag<NBTTagCompound>(NbtCompound)->GetData(entry);
	}
	NBTArray<jint>& NBTHelper::GetIntArray(NBTEntry& entry) {
		return GetTag<NBTTagIntArray>(NbtIntArray)->GetData(entry);
	}

	void NBTHelper::SetDouble(NBTEntry& entry, jdouble value) {
		GetTag<NBTTagDouble>(NbtDouble)->SetData(entry, value);
	}

	void NBTHelper::SetString(NBTEntry& entry, QString value) {
		GetTag<NBTTagString>(NbtString)->SetData(entry, value);
	}
	
}