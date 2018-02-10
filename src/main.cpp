/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: franz
 *
 * Created on 17. Dezember 2017, 16:24
 */

#include <QApplication>
#include "UI/MainForm.h"

#include "Globals.h"
#include "Exception/Exception.h"
#include "Exception/StreamOverflowException.h"
#include "File/GzipByteReader.h"
#include "File/ByteBuffer.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "NBT/NBTReader.h"

#include "UI/NBTTreeModel.h"
#include <QDirModel>
#include <QTranslator>

using namespace std;
using namespace File;

int main(int argc, char *argv[]) {
	// initialize resources, if needed
	// Q_INIT_RESOURCE(resfile);

	QApplication app(argc, argv);

	QTranslator qtTranslator;
	qtTranslator.load(":/languages/lang_" + QLocale::system().name());
	app.installTranslator(&qtTranslator);
	
	UI::MainForm form;
	form.show();

	if (QCoreApplication::arguments().count() >= 2) {
		QString fileArgument = QCoreApplication::arguments().at(1);
		if (!fileArgument.isEmpty())
			form.loadFile(fileArgument);
	}

	return app.exec();
}
