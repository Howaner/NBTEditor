#include "EditArrayDialog.h"
#include <QString>
#include <QTextStream>

namespace UI {
	EditArrayDialog::EditArrayDialog(QWidget* parent) : QDialog(parent) {
		widget.setupUi(this);
		setModal(true);

		connect(widget.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	}

	EditArrayDialog::~EditArrayDialog() {
	}

	void EditArrayDialog::setTitle(QString elementName) {
		setWindowTitle("Bearbeiten: " + elementName);
	}

	template<typename T> void EditArrayDialog::setArray(NBT::NBTArray<T>& array) {
		QString text;
		QTextStream stream(&text);
		for (uint i = 0; i < array.length; i++) {
			stream << array.array[i] << ' ';
		}
		stream.flush();

		widget.plainTextEdit->setPlainText(text);
	}

	template void EditArrayDialog::setArray<jbyte>(NBT::NBTArray<jbyte>&);
	template void EditArrayDialog::setArray<jint>(NBT::NBTArray<jint>&);

	NBT::NBTArray<jbyte>* EditArrayDialog::getResultByteArray() {
		QString str = widget.plainTextEdit->toPlainText();
		QStringList list = str.split(" ", QString::SkipEmptyParts);

		NBT::NBTArray<jbyte>* nbtArray = new NBT::NBTArray<jbyte>();
		nbtArray->length = list.length();
		nbtArray->array = new jbyte[list.length()];

		for (uint i = 0; i < nbtArray->length; i++) {
			bool success = false;
			short number = list[i].toShort(&success, 10);
			if (!success || number < -128 || number > 127) {
				delete nbtArray;
				return NULL;
			}

			nbtArray->array[i] = (jbyte) number;
		}

		return nbtArray;
	}

	NBT::NBTArray<jint>* EditArrayDialog::getResultIntArray() {
		QString str = widget.plainTextEdit->toPlainText();
		QStringList list = str.split(" ", QString::SkipEmptyParts);

		NBT::NBTArray<jint>* nbtArray = new NBT::NBTArray<jint>();
		nbtArray->length = list.length();
		nbtArray->array = new jint[list.length()];

		for (uint i = 0; i < nbtArray->length; i++) {
			bool success = false;
			int number = list[i].toInt(&success, 10);
			if (!success) {
				delete nbtArray;
				return NULL;
			}

			nbtArray->array[i] = number;
		}

		return nbtArray;
	}

	void EditArrayDialog::clickOk() {
		accept();
	}
	
}