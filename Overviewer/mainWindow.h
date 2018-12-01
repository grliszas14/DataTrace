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

#include <QMainWindow>
#include <QFrame>

class MainWindow: public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
	void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private:
	void createMenus();

	QMenu* fileMenu_;
	QMenu* editMenu_;
	QMenu* helpMenu_;
	QFrame* graphsFrame_;
	QFrame* rightPanel_;
};

#endif // MAINWINDOW_H
