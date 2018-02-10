#pragma once
#include "ui_AboutDialog.h"

namespace UI {
	class AboutDialog : public QDialog {
		Q_OBJECT
	public:
		AboutDialog(QWidget* parent);
		virtual ~AboutDialog();

	private:
		Ui::AboutDialog widget;
	};
}