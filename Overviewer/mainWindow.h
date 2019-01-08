/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.h
 *
 *    Description:
 *
 *        Authors:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <memory>
#include "controlPanel.h"
#include "../LibraryDT/param.h"

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
	int CountParamsInConfig();
	void ParseConfig();

	// GUI attributes
	QMenu* fileMenu_;
	QMenu* editMenu_;
	QMenu* helpMenu_;
	QFrame* graphsFrame_;
	ControlPanel* rightPanel_;

	// Parser attributes
	std::unique_ptr<Param[]> parsedParameters;

	// Data sets attributes
	int numberOfQueryRows;
	std::unique_ptr<int[]> dataSeriesValue;
	std::unique_ptr<QDateTime[]> dataSeriesTimestamp;
};

#endif // MAINWINDOW_H
