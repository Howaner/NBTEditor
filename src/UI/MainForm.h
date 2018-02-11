#pragma once
#include "ui_MainForm.h"
#include "TreeContextMenu.h"
#include <QString>
#include <QDropEvent>
#include "NBT/NBTType.h"

namespace UI {
	class MainForm : public QMainWindow {
		Q_OBJECT
	public:
		MainForm();
		virtual ~MainForm();

		QTreeView* GetTreeView() { return widget.treeView; }
		Ui::MainForm& GetWidget() { return widget; }

		void loadFile(QString file);
		void saveFile(QString file);

		virtual void dragEnterEvent(QDragEnterEvent* event) override { event->acceptProposedAction(); }
		virtual void dragMoveEvent(QDragMoveEvent* event) override { event->acceptProposedAction(); }
		virtual void dragLeaveEvent(QDragLeaveEvent* event) override { event->accept(); }
		virtual void dropEvent(QDropEvent* event) override;
	
	public slots:
		void onActionNew();
		void onActionOpen();
		void onActionSave();
		void onActionSaveAs();
		void onActionAbout();

		void onTreeViewDataChanged();

	private:
		Ui::MainForm widget;
		TreeContextMenu* contextMenu;
		
		QString currentFile;
		NBT::NBTFileType currentFileType;
		bool saveEnabled;

		void enableSaving();
		void disableSaving();
	};
}