#include "AboutDialog.h"
#include "Version.h"

namespace UI {
	AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent) {
		widget.setupUi(this);
		setModal(true);
		setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

		widget.nameLabel->setText(widget.nameLabel->text().arg(NBTEDITOR_VERSION));
	}

	AboutDialog::~AboutDialog() {
	}
}
