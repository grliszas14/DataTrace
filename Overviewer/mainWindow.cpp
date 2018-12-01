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
	QMainWindow(parent)
{
	QWidget *window = new QWidget;
	setCentralWidget(window);
	window->resize(800,600);

	fileMenu_ = new QMenu();
	graphsFrame_ = new QFrame();  //TODO
	rightPanel_ = new QFrame();	  //TODO

	createMenus();

	QHBoxLayout * mainLayout = new QHBoxLayout();
	mainLayout->addWidget(graphsFrame_);
	mainLayout->addWidget(rightPanel_);

	window->setLayout(mainLayout);
	setWindowTitle(tr("Overviewer"));
	window->show();
}

void MainWindow::createMenus()
{
	fileMenu_ = menuBar()->addMenu(tr("&File"));
	editMenu_ = menuBar()->addMenu(tr("&Edit"));
	helpMenu_ = menuBar()->addMenu(tr("&Help"));
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

MainWindow::~MainWindow()
{
	delete fileMenu_;
	delete graphsFrame_;
	delete rightPanel_;
}

