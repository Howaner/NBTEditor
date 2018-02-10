#include "TreeContextMenu.h"
#include "MainForm.h"
#include "NBTTreeModel.h"
#include "NBTTreeItem.h"
#include "AddElementDialog.h"
#include "EditArrayDialog.h"
#include "NBT/NBTEntry.h"
#include "NBT/NBTHelper.h"
#include "NBT/NBTTag.h"
#include <iostream>
#include <QMessageBox>

using namespace NBT;

namespace UI {
	TreeContextMenu::TreeContextMenu(MainForm* form) : QObject(), form(form) {
		InitActions();

		form->GetTreeView()->setContextMenuPolicy(Qt::CustomContextMenu);
		QWidget::connect(form->GetTreeView(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OpenContextMenu(const QPoint&)));
		QWidget::connect(form->GetTreeView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OnDoubleClick(QModelIndex)));
	}

	TreeContextMenu::~TreeContextMenu() {
		form = NULL;
	}

	QMenu* TreeContextMenu::GetContextMenu(QModelIndex& index) {
		addToCompoundAction->setVisible(false);
		editAction->setVisible(false);

		NBTTreeItem* item = GetModel()->GetItem(index);
		if (item->nbtEntry != NULL) {
			if (item->nbtEntry->type == NbtCompound || item->nbtEntry->type == NbtList)
				addToCompoundAction->setVisible(true);

			if (item->nbtEntry->type == NbtByteArray || item->nbtEntry->type == NbtIntArray)
				editAction->setVisible(true);
		}

		menu->setProperty("index", QVariant(index));
		return menu;
	}

	void TreeContextMenu::OpenContextMenu(const QPoint& point) {
		QModelIndex index = form->GetTreeView()->indexAt(point);
		if (index.isValid()) {
			QMenu* menu = GetContextMenu(index);
			if (menu != NULL) {
				menu->exec(form->GetTreeView()->viewport()->mapToGlobal(point));
			}
		}
	}

	void TreeContextMenu::OnDoubleClick(QModelIndex index) {
		NBTTreeItem* item = GetModel()->GetItem(index);
		if (item->nbtEntry == NULL)
			return;

		if (item->nbtEntry->type == NbtByteArray || item->nbtEntry->type == NbtIntArray)
			editArray(index, item);
	}

	void TreeContextMenu::InitActions() {
		menu = new QMenu(form);

		addToCompoundAction = menu->addAction(QIcon::fromTheme("list-add", QIcon(":/icons/icon-add.png")), tr("Add to element"));
		QWidget::connect(addToCompoundAction, SIGNAL(triggered()), this, SLOT(onActionAddToCompound()));

		addAction = menu->addAction(QIcon::fromTheme("go-down", QIcon(":/icons/icon-insertbelow.png")), tr("Insert below"));
		QWidget::connect(addAction, SIGNAL(triggered()), this, SLOT(onActionAdd()));

		editAction = menu->addAction(QIcon::fromTheme("edit", QIcon(":/icons/icon-edit.png")), tr("Edit"));
		QWidget::connect(editAction, SIGNAL(triggered()), this, SLOT(onActionEdit()));

		removeAction = menu->addAction(QIcon::fromTheme("edit-delete", QIcon(":/icons/icon-delete.png")), tr("Remove"));
		QWidget::connect(removeAction, SIGNAL(triggered()), this, SLOT(onActionRemove()));
	}

	NBTTreeModel* TreeContextMenu::GetModel() {
		return static_cast<NBTTreeModel*>(form->GetTreeView()->model());
	}

	void TreeContextMenu::onActionAddToCompound() {
		QModelIndex index = menu->property("index").toModelIndex();
		NBTTreeItem* item = GetModel()->GetItem(index);
		if (item->nbtEntry == NULL)
			return;

		switch (item->nbtEntry->type) {
			case NbtCompound:
				addToCompound(index, -1);
				break;
			case NbtList:
				addToList(index, -1);
				break;
			default:
				break;
		}
	}

	void TreeContextMenu::onActionAdd() {
		QModelIndex index = menu->property("index").toModelIndex();
		QModelIndex parent = GetModel()->parent(index);

		NBTTreeItem* parentItem = GetModel()->GetItem(parent);
		if (parentItem->nbtEntry == NULL)
			return;

		switch (parentItem->nbtEntry->type) {
			case NbtList:
			{
				addToList(parent, index.row() + 1);
				break;
			}
			case NbtCompound:
			{
				addToCompound(parent, index.row() + 1);
				break;
			}
			default:
				break;
		}
	}

	void TreeContextMenu::onActionEdit() {
		QModelIndex index = menu->property("index").toModelIndex();
		NBTTreeItem* item = GetModel()->GetItem(index);
		if (item->nbtEntry == NULL)
			return;

		if (item->nbtEntry->type == NbtByteArray || item->nbtEntry->type == NbtIntArray)
			editArray(index, item);
	}

	void TreeContextMenu::onActionRemove() {
		QModelIndex index = menu->property("index").toModelIndex();
		if (!index.isValid())
			return;

		NBTTreeItem* item = GetModel()->GetItem(index);
		if (item != NULL) {
			GetModel()->RemoveItem(item);
		}
	}

	void TreeContextMenu::addToCompound(const QModelIndex& parent, int position) {
		NBTTreeItem* parentItem = GetModel()->GetItem(parent);

		AddElementDialog dialog(form);
		if (dialog.exec() == QDialog::Accepted) {
			NBTCompound* compound = NBTHelper::GetCompound(*parentItem->nbtEntry);
			if (compound->FindName(dialog.getInputName()) != NULL) {
				QMessageBox::warning(form, tr("Element already exists"), tr("A element with the name %1 is already existing", "").arg(dialog.getInputName()), QMessageBox::Ok, QMessageBox::Ok);
				return;
			}

			NBTEntry* entry = new NBTEntry(dialog.getInputName(), dialog.getInputType());

			const NBTTag* tag = NBTHelper::GetTagByType(dialog.getInputType());
			entry->value = tag->CreateDefaultData();

			NBTHelper::GetCompound(*parentItem->nbtEntry)->AddEntry(entry, position);
			GetModel()->AddItem(entry, parent, position);
		}
	}

	void TreeContextMenu::addToList(const QModelIndex& parent, int position) {
		NBTTreeItem* parentItem = GetModel()->GetItem(parent);
		NBTList& list = NBTHelper::GetList(*parentItem->nbtEntry);

		if (list.type == NbtEnd) {
			AddElementDialog dialog(form);
			dialog.disableInputName();
			if (dialog.exec() != QDialog::Accepted)
				return;

			list.type = dialog.getInputType();
		}

		NBTEntry* entry = new NBTEntry("", list.type);
		const NBTTag* tag = NBTHelper::GetTagByType(list.type);
		entry->value = tag->CreateDefaultData();

		if (position == -1)
			list.entries.push_back(entry);
		else
			list.entries.insert(list.entries.begin() + position, entry);
		GetModel()->AddItem(entry, parent, position);
	}

	void TreeContextMenu::editArray(const QModelIndex& index, NBTTreeItem* item) {
		EditArrayDialog dialog(form);
		dialog.setTitle(item->qtName.toString());

		if (item->nbtEntry->type == NbtByteArray) {
			dialog.setArray(NBTHelper::GetByteArray(*item->nbtEntry));
			if (dialog.exec() == QDialog::Accepted) {
				NBT::NBTArray<jbyte>* newArray = dialog.getResultByteArray();
				if (newArray == NULL) {
					QMessageBox::critical(form, tr("Invalid data"), tr("The input data is invalid.", ""), QMessageBox::Ok, QMessageBox::Ok);
					return;
				}

				NBTHelper::GetTag<NBTTagByteArray>(NbtByteArray)->SetData(*item->nbtEntry, *newArray);
				GetModel()->UpdateItem(index);
			}
		} else if (item->nbtEntry->type == NbtIntArray) {
			dialog.setArray(NBTHelper::GetIntArray(*item->nbtEntry));
			if (dialog.exec() == QDialog::Accepted) {
				NBT::NBTArray<jint>* newArray = dialog.getResultIntArray();
				if (newArray == NULL) {
					QMessageBox::critical(form, tr("Invalid data"), tr("The input data is invalid.", ""), QMessageBox::Ok, QMessageBox::Ok);
					return;
				}

				NBTHelper::GetTag<NBTTagIntArray>(NbtIntArray)->SetData(*item->nbtEntry, *newArray);
				GetModel()->UpdateItem(index);
			}
		}
	}

}