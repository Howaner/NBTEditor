#pragma once

#include "Globals.h"
#include "NBTEntry.h"
#include "File/ByteBuffer.h"
#include "File/WriteBuffer.h"
#include "Exception/NBTException.h"
#include "NBTCompound.h"
#include "NBTHelper.h"
#include <string>
#include <QVariant>
#include <QString>

using namespace File;

namespace NBT {

	class NBTTag {
	public:
		NBTTag() { }

		virtual void* Read(ByteBuffer* buffer) const = 0;

		virtual void Write(WriteBuffer* buffer, NBTEntry& entry) const = 0;

		virtual void* CreateDefaultData() const = 0;

		virtual QString GetQtIconName() const = 0;

		virtual QString GetTypeName() const = 0;

		virtual QVariant GetQtData(NBTEntry& entry) const = 0;

		virtual bool SetQtData(NBTEntry& entry, const QVariant& value) const = 0;

		virtual void Free(NBTEntry& entry) const = 0;
	};

	template<typename T> class NBTTagBasic : public NBTTag {
	public:
		NBTTagBasic(const char* iconName, const char* typeName) : NBTTag(), qtIconName(QString(iconName)), typeName(QString(typeName)) {};
		NBTTagBasic() : NBTTag(), qtIconName(), typeName() {}

		virtual T& GetData(NBTEntry& entry) const {
			return *(static_cast<T*>(entry.value));
		}

		virtual void SetData(NBTEntry& entry, T& data) const {
			if (entry.value == NULL)
				entry.value = new T;
			*(static_cast<T*>(entry.value)) = data;
		}

		virtual QString GetQtIconName() const override {
			return qtIconName;
		}

		virtual QString GetTypeName() const override {
			return typeName;
		}

		virtual QVariant GetQtData(NBTEntry& entry) const override = 0;

		virtual bool SetQtData(NBTEntry& entry, const QVariant& value) const override = 0;

		virtual void* Read(ByteBuffer* buffer) const override = 0;

		virtual void Write(WriteBuffer* buffer, NBTEntry& entry) const = 0;

		virtual void* CreateDefaultData() const override = 0;

		virtual void Free(NBTEntry& entry) const override {
			if (entry.value != NULL) {
				delete static_cast<T*>(entry.value);
				entry.value = NULL;
			}
		}
		
	private:
		QString qtIconName;
		QString typeName;
	};

	class NBTTagByte : public NBTTagBasic<jbyte> {
	public:
		NBTTagByte() : NBTTagBasic("nbt-byte.png", "Byte") {}

		void* Read(ByteBuffer* buffer) const override {
			jbyte* data = new jbyte;
			*data = buffer->ReadSignedByte();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteSignedByte(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new jbyte(0);
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry));
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			bool success = false;
			short number = value.toString().toShort(&success, 10);
			if (!success)
				return false;

			if (number < -128 || number > 127)
				return false;

			SetData(entry, (jbyte&) number);
			return true;
		}
	};

	class NBTTagShort : public NBTTagBasic<jshort> {
	public:
		NBTTagShort() : NBTTagBasic("nbt-short.png", "Short") {}

		void* Read(ByteBuffer* buffer) const override {
			short* data = new short;
			*data = buffer->ReadShort();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteShort(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new jshort(0);
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry));
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			bool success = false;
			short number = value.toString().toShort(&success, 10);
			if (!success)
				return false;

			SetData(entry, number);
			return true;
		}
	};

	class NBTTagInt : public NBTTagBasic<jint> {
	public:
		NBTTagInt() : NBTTagBasic("nbt-int.png", "Int") {}

		void* Read(ByteBuffer* buffer) const override {
			int* data = new int;
			*data = buffer->ReadInt();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteInt(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new jint(0);
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry));
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			bool success = false;
			int number = value.toString().toInt(&success, 10);
			if (!success)
				return false;

			SetData(entry, number);
			return true;
		}
	};

	class NBTTagLong : public NBTTagBasic<jlong> {
	public:
		NBTTagLong() : NBTTagBasic("nbt-long.png", "Long") {}

		void* Read(ByteBuffer* buffer) const override {
			jlong* data = new jlong;
			*data = buffer->ReadLong();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteLong(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new jlong(0);
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString("%1").arg(GetData(entry));
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			bool success = false;
			jlong number = (sizeof(long) == 8) ? value.toString().toLong(&success, 10) : value.toString().toLongLong(&success, 10);
			if (!success)
				return false;

			SetData(entry, number);
			return true;
		}
	};

	class NBTTagFloat : public NBTTagBasic<jfloat> {
	public:
		NBTTagFloat() : NBTTagBasic("nbt-float.png", "Float") {}

		void* Read(ByteBuffer* buffer) const override {
			jfloat* data = new jfloat;
			*data = buffer->ReadFloat();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteFloat(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new jfloat(0);
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry), 'g', 6);
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			bool success = false;
			float number = value.toString().toFloat(&success);
			if (!success)
				return false;

			SetData(entry, number);
			return true;
		}
	};

	class NBTTagDouble : public NBTTagBasic<jdouble> {
	public:
		NBTTagDouble() : NBTTagBasic("nbt-double.png", "Double") {}

		void* Read(ByteBuffer* buffer) const override {
			jdouble* data = new jdouble;
			*data = buffer->ReadDouble();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteDouble(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new jdouble(0);
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry), 'g', 13);
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			bool success = false;
			jdouble number = (jdouble) value.toString().toDouble(&success);
			if (!success)
				return false;

			SetData(entry, number);
			return true;
		}
	};

	class NBTTagByteArray : public NBTTagBasic<NBTArray<jbyte>> {
	public:
		NBTTagByteArray() : NBTTagBasic("nbt-array.png", "Byte Array") {}

		void* Read(ByteBuffer* buffer) const override {
			jint length = buffer->ReadInt();
			// TODO: Overflow check

			jbyte* array = new jbyte[length];
			for (jint i = 0; i < length; i++) {
				array[i] = buffer->ReadByte();
			}

			return new NBTArray<jbyte>(length, array);
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			NBTArray<jbyte>& data = GetData(entry);
			buffer->WriteInt(data.length);

			for (uint i = 0; i < data.length; i++) {
				buffer->WriteByte(data.array[i]);
			}
		}

		void* CreateDefaultData() const override {
			return new NBTArray<jbyte>(0, NULL);
		}

		void SetData(NBTEntry& entry, NBTArray<jbyte>& data) const override {
			if (entry.value != NULL)
				delete static_cast<NBTArray<jbyte>*>(entry.value);
			entry.value = &data;
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry).length) + QString(" bytes");
		}

		bool SetQtData(NBTEntry&, const QVariant&) const override {
			// Not possible
			return false;
		}
	};

	class NBTTagIntArray : public NBTTagBasic<NBTArray<jint>> {
	public:
		NBTTagIntArray() : NBTTagBasic("nbt-array.png", "Int Array") {}

		void* Read(ByteBuffer* buffer) const override {
			jint length = buffer->ReadInt();
			// TODO: Overflow check

			int* array = new int[length];
			for (jint i = 0; i < length; i++) {
				array[i] = buffer->ReadInt();
			}

			return new NBTArray<jint>(length, array);
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			NBTArray<jint>& data = GetData(entry);
			buffer->WriteInt(data.length);

			for (uint i = 0; i < data.length; i++) {
				buffer->WriteInt(data.array[i]);
			}
		}

		void* CreateDefaultData() const override {
			return new NBTArray<jint>(0, new jint[0]);
		}

		void SetData(NBTEntry& entry, NBTArray<jint>& data) const override {
			if (entry.value != NULL)
				delete static_cast<NBTArray<jint>*>(entry.value);
			entry.value = &data;
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry).length) + QString(" ints");
		}

		bool SetQtData(NBTEntry&, const QVariant&) const override {
			// Not possible
			return false;
		}
	};

	class NBTTagLongArray : public NBTTagBasic<NBTArray<jlong>> {
	public:
		NBTTagLongArray() : NBTTagBasic("nbt-array.png", "Long Array") {}

		void* Read(ByteBuffer* buffer) const override {
			jint length = buffer->ReadInt();
			// TODO: Overflow check

			long* array = new long[length];
			for (jint i = 0; i < length; i++) {
				array[i] = buffer->ReadLong();
			}

			return new NBTArray<jlong>(length, array);
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			NBTArray<jlong>& data = GetData(entry);
			buffer->WriteInt(data.length);

			for (uint i = 0; i < data.length; i++) {
				buffer->WriteLong(data.array[i]);
			}
		}

		void* CreateDefaultData() const override {
			return new NBTArray<jlong>(0, new jlong[0]);
		}

		void SetData(NBTEntry& entry, NBTArray<jlong>& data) const override {
			if (entry.value != NULL)
				delete static_cast<NBTArray<jlong>*>(entry.value);
			entry.value = &data;
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return QString::number(GetData(entry).length) + QString(" longs");
		}

		bool SetQtData(NBTEntry&, const QVariant&) const override {
			// Not possible
			return false;
		}
	};

	class NBTTagList : public NBTTagBasic<NBTList> {
	public:
		NBTTagList() : NBTTagBasic("nbt-list.png", "List") {}

		void* Read(ByteBuffer* buffer) const override {
			NBTType type = static_cast<NBTType>(buffer->ReadByte());
			jint length = buffer->ReadInt();

			if (type == NbtEnd) {
				if (length != 0)
					throw Exception::NBTException("Missing type in list", NULL);
				return new NBTList(type);
			}

			const NBTTag* tag = NBTHelper::GetTagByType(type);
			if (tag == NULL)
				throw Exception::NBTException("Unknown nbt element type: " + std::to_string(type), NULL);

			NBTList* list = new NBTList(type);
			for (jint i = 0; i < length; i++) {
				QString name = QString(i);
				NBTEntry* entry = new NBTEntry(name, type);
				entry->value = tag->Read(buffer);

				list->entries.push_back(entry);
			}
			return list;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			NBTList& list = GetData(entry);
			buffer->WriteByte(list.type);
			buffer->WriteInt((jint) list.entries.size());

			if (list.type != NbtEnd) {
				const NBTTag* tag = NBTHelper::GetTagByType(list.type);
				if (tag == NULL)
					throw Exception::NBTException("Unknown nbt element type: " + std::to_string(list.type), NULL);

				for (auto& entry : list.entries) {
					tag->Write(buffer, *entry);
				}
			}
		}

		void* CreateDefaultData() const override {
			return new NBTList(NbtEnd);
		}

		void SetData(NBTEntry& entry, NBTList& data) const override {
			entry.value = &data;
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			NBTList& list = GetData(entry);
			const NBTTag* tag = NBTHelper::GetTagByType(list.type);

			QString typeName = (tag == NULL ? "unknown" : tag->GetTypeName());
			return QString::number(GetData(entry).entries.size()) + QString(" entries (") + typeName + ")";
		}

		bool SetQtData(NBTEntry&, const QVariant&) const override {
			// Not possible
			return false;
		}
	};

	class NBTTagString : public NBTTagBasic<QString> {
	public:
		NBTTagString() : NBTTagBasic("nbt-string.png", "String") {}

		void* Read(ByteBuffer* buffer) const override {
			QString* data = new QString();
			*data = buffer->ReadString();
			return data;
		}

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override {
			buffer->WriteString(GetData(entry));
		}

		void* CreateDefaultData() const override {
			return new QString();
		}

		void SetData(NBTEntry& entry, QString& data) const override {
			if (entry.value == NULL)
				entry.value = new QString;
			*(static_cast<QString*>(entry.value)) = data;
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			return GetData(entry);
		}

		bool SetQtData(NBTEntry& entry, const QVariant& value) const override {
			if (value.userType() != QMetaType::QString)
				return false;

			QString str = value.toString();
			SetData(entry, str);
			return true;
		}
	};

	class NBTTagCompound : public NBTTag {
	public:
		NBTTagCompound() : NBTTag() {}

		NBTCompound* GetData(NBTEntry& entry) const {
			return static_cast<NBTCompound*>(entry.value);
		}

		void* CreateDefaultData() const override {
			return new NBTCompound();
		}

		void SetData(NBTEntry& entry, NBTCompound* data) const {
			entry.value = data;
		}

		QString GetQtIconName() const override {
			return QString("nbt-compound.png");
		}

		QString GetTypeName() const override {
			return QString("Compound");
		}

		QVariant GetQtData(NBTEntry& entry) const override {
			NBTCompound* compound = GetData(entry);
			return QString::number(compound->GetEntries().size()) + QString(" entries");
		}

		bool SetQtData(NBTEntry&, const QVariant&) const override {
			// Not possible
			return false;
		}

		void* Read(ByteBuffer* buffer) const override;  // Defined in NBTCompound.cpp

		void Write(WriteBuffer* buffer, NBTEntry& entry) const override;

		void Free(NBTEntry& entry) const override {
			if (entry.value != NULL) {
				delete static_cast<NBTCompound*>(entry.value);
				entry.value = NULL;
			}
		}
	};

}