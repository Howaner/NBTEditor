#include <QIcon>
#include "NBTTreeModel.h"
#include "NBT/NBTEntry.h"
#include "NBT/NBTCompound.h"
#include "NBT/NBTHelper.h"
#include "NBT/NBTTag.h"
#include "Exception/Exception.h"
#include <iostream>

using namespace NBT;

namespace UI {

	NBTTreeModel::NBTTreeModel(NBT::NBTEntry* rootEntry) : rootItem(NULL), rootEntry(rootEntry) {
		rootItem = CreateTreeItems(rootEntry);
	}


	NBTTreeModel::~NBTTreeModel() {
		delete rootItem;
		rootItem = NULL;

		delete rootEntry;
		rootEntry = NULL;
	}

	QVariant NBTTreeModel::data(const QModelIndex& index, int role) const {
		if (!index.isValid())
			return QVariant();

		if (role == Qt::DecorationRole && index.column() == 0) {
			NBTTreeItem* item = GetItem(index);
			if (item->nbtEntry == NULL)
				return QVariant();

			const NBTTag* tag = NBTHelper::GetTagByType(item->nbtEntry->type);
			QString iconName = tag->GetQtIconName();
			if (iconName.isNull())
				return QVariant();
			else
				return QIcon(":/nbt_icons/" + iconName);
		}

		if (role == Qt::DisplayRole || role == Qt::EditRole) {
			NBTTreeItem* item = GetItem(index);
			NBTType parentType = (item->parentItem != NULL && item->parentItem->nbtEntry != NULL) ? item->parentItem->nbtEntry->type : NbtEnd;
			switch (parentType) {
				case NbtByteArray:
				case NbtIntArray:
				case NbtList:
					return (index.column() == 0 ? item->qtData : QVariant());
				default:
					return (index.column() == 0 ? item->qtName : item->qtData);
			}
		}

		return QVariant();
	}

	QVariant NBTTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
			return QString("Name");
		else if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 1)
			return QString("Value");
		else
			return QVariant();
	}

	QModelIndex NBTTreeModel::index(int row, int column, const QModelIndex& parent) const {
		NBTTreeItem* parentItem = GetItem(parent);

		NBTTreeItem* childItem = parentItem->childItems[row];
		return (childItem ? createIndex(row, column, childItem) : QModelIndex());
	}

	QModelIndex NBTTreeModel::parent(const QModelIndex& index) const {
		if (!index.isValid())
			return QModelIndex();

		NBTTreeItem* childItem = GetItem(index);
		NBTTreeItem* parentItem = childItem->parentItem;
		if (parentItem == NULL || parentItem == rootItem)
			return QModelIndex();

		int row = (parentItem->parentItem != NULL ? parentItem->parentItem->childItems.indexOf(parentItem) : 0);
		return createIndex(row, 0, parentItem);
	}

	int NBTTreeModel::rowCount(const QModelIndex& parent) const {
		NBTTreeItem* parentItem = GetItem(parent);
		return parentItem->childItems.count();
	}

	int NBTTreeModel::columnCount(const QModelIndex& index) const {
		NBTTreeItem* item = GetItem(index);
		if (item->parentItem != NULL && item->parentItem->nbtEntry != NULL) {
			switch (item->parentItem->nbtEntry->type) {
				case NbtByteArray:
				case NbtIntArray:
				case NbtList:
					return 2;
				default:
					break;
			}
		}

		return 2;
	}

	Qt::ItemFlags NBTTreeModel::flags(const QModelIndex& index) const {
		NBTTreeItem* item = GetItem(index);
		if (item->nbtEntry == NULL)
			return QAbstractItemModel::flags(index);

		switch (item->nbtEntry->type) {
			case NbtByte:
			case NbtShort:
			case NbtInt:
			case NbtLong:
			case NbtFloat:
			case NbtDouble:
			case NbtString:
				if (item->parentItem != NULL && item->parentItem->nbtEntry != NULL && item->parentItem->nbtEntry->type == NbtList) {
					if (index.column() == 0)
						return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
				} else if (index.column() == 1)
					return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
			default:
				return QAbstractItemModel::flags(index);
		}
	}

	NBTTreeItem* NBTTreeModel::GetItem(const QModelIndex& index) const {
		if (index.isValid() && index.internalPointer() != NULL)
			return static_cast<NBTTreeItem*>(index.internalPointer());
		else
			return rootItem;
	}

	bool NBTTreeModel::RemoveItem(NBTTreeItem* item) {
		NBTTreeItem* parent = item->parentItem;
		if (parent == NULL)
			return false;

		switch (parent->nbtEntry->type) {
			case NbtCompound:
			{
				NBTCompound* compound = NBTHelper::GetCompound(*parent->nbtEntry);
				if (!compound->RemoveEntry(item->nbtEntry))
					return false;
				break;
			}
			case NbtList:
			{
				NBTList& list = NBTHelper::GetList(*parent->nbtEntry);
				std::vector<NBTEntry*>::iterator pos = std::find(list.entries.begin(), list.entries.end(), item->nbtEntry);
				if (pos == list.entries.end())
					return false;

				list.entries.erase(pos);
				break;
			}
			default:
				return false;
		}

		int row = parent->childItems.indexOf(item);
		if (row != 0 && row == (parent->childItems.count() - 1))
			row -= 1;  // A qt bug causes the application to crash if the last item is going to get removed.

		QModelIndex parentIndex = this->parent(createIndex(row, 0, item));
		beginRemoveRows(parentIndex, row, row);
		parent->childItems.removeOne(item);
		delete item->nbtEntry;
		delete item;
		endRemoveRows();

		UpdateItem(parentIndex);
		return true;
	}
	
	void NBTTreeModel::AddItem(NBT::NBTEntry* entry, const QModelIndex& parent, int position) {
		NBTTreeItem* item = CreateTreeItems(entry);
		AddItem(item, parent, position);
	}

	void NBTTreeModel::AddItem(NBTTreeItem* item, const QModelIndex& parent, int position) {
		NBTTreeItem* parentItem = GetItem(parent);
		if (position < 0)
			position = parentItem->childItems.count();
		item->parentItem = parentItem;

		beginInsertRows(parent, position, position);
		parentItem->childItems.insert(position, item);
		endInsertRows();

		if (parentItem->nbtEntry != NULL) {
			switch (parentItem->nbtEntry->type) {
				case NbtCompound:
				case NbtList:
				case NbtByteArray:
				case NbtIntArray:
				{
					const NBTTag* tag = NBTHelper::GetTagByType(parentItem->nbtEntry->type);
					parentItem->qtData = tag->GetQtData(*parentItem->nbtEntry);
					emit dataChanged(parent, parent);
					break;
				}
				default:
					break;
			}
		}
	}

	bool NBTTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
		if (role != Qt::EditRole || !index.isValid())
			return false;

		NBTTreeItem* model = GetItem(index);
		if (model->nbtEntry == NULL)
			return false;

		const NBTTag* tag = NBTHelper::GetTagByType(model->nbtEntry->type);
		if (!tag->SetQtData(*model->nbtEntry, value))
			return false;

		model->qtData = tag->GetQtData(*model->nbtEntry);
		emit dataChanged(index, index);
		return true;
	}

	void NBTTreeModel::UpdateItem(const QModelIndex& index) {
		NBTTreeItem* model = GetItem(index);
		if (model->nbtEntry == NULL)
			return;

		const NBTTag* tag = NBTHelper::GetTagByType(model->nbtEntry->type);
		model->qtData = tag->GetQtData(*model->nbtEntry);
		emit dataChanged(index, index);
	}

	NBTTreeItem* NBTTreeModel::CreateTreeItems(NBTEntry* entry) {
		const NBTTag* tag = NBTHelper::GetTagByType(entry->type);
		NBTTreeItem* item = new NBTTreeItem(entry, entry->name, tag->GetQtData(*entry));

		int rowIndex = 0;

		switch (entry->type) {
			case NbtCompound:
			{
				NBTCompound* compound = NBTHelper::GetCompound(*entry);
				for (auto& e : compound->GetEntries()) {
					NBTTreeItem* childItem = CreateTreeItems(e);
					childItem->parentItem = item;
					childItem->childNumber = rowIndex++;
					item->childItems.push_back(childItem);
				}
				break;
			}
			case NbtList:
			{
				NBTList& list = NBTHelper::GetList(*entry);
				for (auto& elem : list.entries) {
					NBTTreeItem* childItem = CreateTreeItems(elem);
					childItem->parentItem = item;
					childItem->childNumber = rowIndex++;
					item->childItems.push_back(childItem);
				}
			}
			default:
				break;
		}

		return item;
	}
}
