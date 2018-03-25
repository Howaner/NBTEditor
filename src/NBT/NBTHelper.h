#pragma once
#include "Globals.h"
#include "NBTEntry.h"

namespace NBT {
	class NBTTag;
	class NBTCompound;

	class NBTHelper {
	public:
		static const NBTTag* GetTagByType(NBTType type);
		template<typename T> static const T* GetTag(NBTType type);
		static const NBTTag** GetAllTags();

		static jbyte GetByte(NBTEntry& entry);
		static jshort GetShort(NBTEntry& entry);
		static jint GetInt(NBTEntry& entry);
		static jlong GetLong(NBTEntry& entry);
		static jfloat GetFloat(NBTEntry& entry);
		static jdouble GetDouble(NBTEntry& entry);
		static NBTArray<jbyte>& GetByteArray(NBTEntry& entry);
		static NBTArray<jint>& GetIntArray(NBTEntry& entry);
		static QString& GetString(NBTEntry& entry);
		static NBTList& GetList(NBTEntry& entry);
		static NBTCompound* GetCompound(NBTEntry& entry);

		static void SetDouble(NBTEntry& entry, jdouble value);
		static void SetString(NBTEntry& entry, QString value);

	private:
		static const NBTTag** tagsByType;
	};
}