/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.cpp
 *
 *    Description:
 *
 *        Authors:  Grzegorz Wojciechowski, Michał Skarzyński
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <QtWidgets>
#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent)
{
	button_ = new QPushButton(tr("Push me!"));
	textBrowser_ = new QTextBrowser();

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(button_,0,0);
	mainLayout->addWidget(textBrowser_,1,0);
	setLayout(mainLayout);
	setWindowTitle(tr("Connecting buttons to processes.."));
}

MainWindow::~MainWindow()
{
	delete button_;
	delete textBrowser_;
}

