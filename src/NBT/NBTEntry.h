#pragma once
#include <vector>
#include "NBTType.h"
#include <QModelIndex>
#include <QString>

namespace NBT {

	struct NBTEntry {
		QString name;
		NBTType type;
		void* value;

		NBTEntry(QString name, NBTType type) : name(name), type(type) {}
		~NBTEntry();
	};

	template<typename T> struct NBTArray {
		uint length;
		T* array;

		NBTArray(uint length, T* array) : length(length), array(array) {}
		NBTArray() : length(0), array(NULL) {}
		~NBTArray() {
			length = 0;
			delete[] array;
		}
	};

	struct NBTList {
		NBTType type;
		std::vector<NBTEntry*> entries;

		NBTList(NBTType type) : type(type) {}
		NBTList() : type(NbtEnd) {}
		~NBTList() {
			for (auto& elem : entries) {
				delete elem;
			}
			entries.clear();
			type = NbtEnd;
		}
	};
}