#pragma once
#include "ui_EditArrayDialog.h"
#include "Globals.h"
#include "NBT/NBTEntry.h"
#include <QString>

namespace UI {
	class EditArrayDialog : public QDialog {
		Q_OBJECT
	public:
		EditArrayDialog(QWidget* parent);
		virtual ~EditArrayDialog();

		void setTitle(QString elementName);
		template<typename T> void setArray(NBT::NBTArray<T>& array);

		NBT::NBTArray<jbyte>* getResultByteArray();
		NBT::NBTArray<jint>* getResultIntArray();

	public slots:
		void clickOk();

	private:
		Ui::EditArrayDialog widget;
	};
}