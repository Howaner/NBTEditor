#include "AddElementDialog.h"
#include "NBT/NBTHelper.h"
#include "NBT/NBTTag.h"
#include <iostream>
#include <QPushButton>

namespace UI {
	AddElementDialog::AddElementDialog(QWidget* parent) : QDialog(parent) {
		widget.setupUi(this);
		setModal(true);
		setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

		InitTypeSelection();
		widget.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

		connect(widget.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
		connect(widget.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
		connect(widget.nameField, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
	}

	AddElementDialog::~AddElementDialog() {
	}

	QString AddElementDialog::getInputName() {
		return widget.nameField->text();
	}

	NBT::NBTType AddElementDialog::getInputType() {
		QVariant selectedData = widget.typeField->currentData();
		if (!selectedData.isValid() || selectedData.isNull())
			return NBT::NbtEnd;

		bool success = false;
		int convertedInt = selectedData.toInt(&success);
		if (!success)
			return NBT::NbtEnd;

		return static_cast<NBT::NBTType>(convertedInt);
	}

	void AddElementDialog::disableInputName() {
		widget.nameField->setEnabled(false);
	}

	void AddElementDialog::InitTypeSelection() {
		const NBT::NBTTag** tags = NBT::NBTHelper::GetAllTags();
		for (uint i = MIN_TAG; i <= MAX_TAG; i++) {
			const NBT::NBTTag* tag = tags[i];
			if (tag == NULL)
				continue;

			QString typeName = tag->GetTypeName();
			if (typeName.isEmpty())
				continue;

			QString iconName = tag->GetQtIconName();
			if (iconName.isNull())
				widget.typeField->addItem(typeName, QVariant(i));
			else
				widget.typeField->addItem(QIcon(":/nbt_icons/" + iconName), typeName, QVariant(i));
		}
	}

	void AddElementDialog::onTextChanged(const QString& text) {
		if (text.isEmpty()) {
			widget.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		} else {
			widget.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		}
	}
}
