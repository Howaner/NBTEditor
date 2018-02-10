#pragma once
#include <QList>
#include <QVariant>
#include "NBT/NBTEntry.h"

namespace UI {
	struct NBTTreeItem {
		NBTTreeItem() : nbtEntry(NULL), parentItem(NULL), childNumber(0) {}
		NBTTreeItem(NBT::NBTEntry* nbtEntry, QVariant qtName, QVariant qtData) : nbtEntry(nbtEntry), parentItem(NULL), qtName(qtName), qtData(qtData), childNumber(0) {}

		~NBTTreeItem() {
			for (auto& child : childItems) {
				delete child;
			}

			parentItem = NULL;
			qtName.clear();
			qtData.clear();
			childItems.clear();
		}

		NBT::NBTEntry* nbtEntry;
		NBTTreeItem* parentItem;
		QVariant qtName;
		QVariant qtData;

		QList<NBTTreeItem*> childItems;
		int childNumber;
	};
}