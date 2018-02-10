#pragma once
#include "NBTTreeItem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QWidget>

namespace NBT {
	class NBTEntry;
}

namespace UI {
	class NBTTreeModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		NBTTreeModel(NBT::NBTEntry* rootEntry);
		~NBTTreeModel();

		QVariant data(const QModelIndex& index, int role) const override;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& index) const override;

		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const override;

		Qt::ItemFlags flags(const QModelIndex& index) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

		NBT::NBTEntry* GetRootEntry() const { return rootEntry; }
		NBTTreeItem* GetItem(const QModelIndex& index) const;
		bool RemoveItem(NBTTreeItem* item);
		void AddItem(NBT::NBTEntry* entry, const QModelIndex& parent, int position = -1);
		void AddItem(NBTTreeItem* item, const QModelIndex& parent, int position = -1);

		/** Forces a window update of the specific element. */
		void UpdateItem(const QModelIndex& index);

	protected:
		NBTTreeItem* rootItem;
		NBT::NBTEntry* rootEntry;

	private:
		NBTTreeItem* CreateTreeItems(NBT::NBTEntry* rootEntry);
	};
}