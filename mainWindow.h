/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.h
 *
 *    Description:
 *
 *        Authors:  Grzegorz Wojciechowski, Michał Skarzyński
 *
 * =====================================================================================
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QPushButton;
class QTextBrowser;

class MainWindow: public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QPushButton* button_;
	QTextBrowser* textBrowser_;
};

#endif // MAINWINDOW_H
