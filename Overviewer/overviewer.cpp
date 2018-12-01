/*
 * =====================================================================================
 *
 *       Filename:  overviewer.cpp
 *
 *    Description:  Main app for overviewing gathered data
 *
 *        Authors:  Grzegorz Wojciechowski, Michał Skarzyński
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <QApplication>
#include <QtWidgets>
#include "mainWindow.h"

int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	MainWindow w;
	w.show();

	return app.exec();
}
