#pragma once
#include "ui_AddElementDialog.h"
#include "NBT/NBTType.h"

namespace UI {
	class AddElementDialog : public QDialog {
		Q_OBJECT
	public:
		AddElementDialog(QWidget* parent);
		virtual ~AddElementDialog();

		QString getInputName();
		NBT::NBTType getInputType();

		void disableInputName();

	public slots:
		void onTextChanged(const QString& text);

	private:
		Ui::AddElementDialog widget;

		void InitTypeSelection();
	};
}
