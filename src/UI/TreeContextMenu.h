#pragma once
#include <QObject>
#include <QModelIndex>
#include <QPoint>
#include <QMenu>

namespace UI {
	class MainForm;
	class NBTTreeModel;
	class NBTTreeItem;

	class TreeContextMenu : public QObject {
		Q_OBJECT

	public:
		TreeContextMenu(MainForm* form);
		~TreeContextMenu();

		QMenu* GetContextMenu(QModelIndex& index);

	public slots:
		void OpenContextMenu(const QPoint& point);
		void onActionAddToCompound();
		void onActionAdd();
		void onActionRemove();
		void onActionEdit();

		void OnDoubleClick(QModelIndex index);

	protected:
		void addToCompound(const QModelIndex& parent, int position = -1);
		void addToList(const QModelIndex& parent, int position = -1);

		void editArray(const QModelIndex& index, NBTTreeItem* item);

	private:
		void InitActions();
		NBTTreeModel* GetModel();

		QAction* addToCompoundAction;
		QAction* addAction;
		QAction* removeAction;
		QAction* editAction;

		QMenu* menu;
		MainForm* form;
	};
}