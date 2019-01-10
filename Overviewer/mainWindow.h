/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.h
 *
 *    Description:  App made for overviewing data gathered by DataTrace system.
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
#include <QtCharts>
#include <vector>
#include <QtSql>
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

private slots:
	void ScrollLeftFunc();
	void ScrollRightFunc();

private:
	// GUI attributes
	QMenu* fileMenu_;
	QMenu* editMenu_;
	QMenu* helpMenu_;
	QPushButton* scrollLeft_;
	QPushButton* scrollRight_;
	QFrame* graphsFrame_;
	ControlPanel* rightPanel_;
	QChart* chart;
	QChartView *chartView;
	QDateTimeAxis *axisX;
	QValueAxis *axisY;
	int maxToDisplay, minToDisplay;
	QDateTime leftBorder, rightBorder;
	QString chartTitle;
	bool dayOrNight = true;

	// Parser attributes
	std::unique_ptr<Param[]> parsedParameters;
	std::vector<QString> seriesSets;

	// Query attributes
	QSqlDatabase db;
	int numOfParams;
	typedef enum {
		DEFAULT,
		LEFT,
		RIGHT } TypeOfQuery;

	// Data sets attributes
	int numberOfQueryRows;
	QLineSeries *series_[5];
	QString legend[5] = {"", "", "", "", ""};
	std::vector<std::unique_ptr<int[]>> controlVector;
	std::unique_ptr<int[]> dataSeriesValue;
	std::unique_ptr<QDateTime[]> dataSeriesTimestamp;

	// Functions
	void createMenus();
	int CountParamsInConfig();
	void ParseConfig();
	void GetDataSeries(TypeOfQuery);
	void SetupAndDrawCharts();
};

#endif // MAINWINDOW_H
