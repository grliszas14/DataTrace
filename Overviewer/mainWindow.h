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
	/** Default constructor. Creates main window of Overviewer app. */
	explicit MainWindow(QWidget *parent = 0);
	/** Default destructor. */
	~MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
	void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
	/** Function connected to arrow below charts widget. Scrolls charts to the left. */
	void ScrollLeftFunc();
	/** Function connected to arrow below charts widget. Scrolls charts to the right. */
	void ScrollRightFunc();

private:
	// GUI attributes
	QMenu* fileMenu_;									/** Expandable file menu from toolbar. */
	QMenu* editMenu_;									/** Expandable edit menu from toolbar. */
	QMenu* helpMenu_;									/** Expandable help menu from toolbar. */
	QPushButton* scrollLeft_;							/** Arrow allowing scrolling charts. */
	QPushButton* scrollRight_;							/** Arrow allowing scrolling charts. */
	ControlPanel* rightPanel_;							/** Panel on the right side of app. */
	QChart* chart;										/** Widget managing graphical representation of the chart's series, legend and axes. */
	QChartView *chartView;								/** Standalone widget that can display charts. */
	QDateTimeAxis *axisX;								/** Represents time axis on chart. */
	QValueAxis *axisY;									/** Represents value axis on the chart. */
	int maxToDisplay, minToDisplay;						/** Represents boundaries of chart's values. */
	QDateTime leftBorder, rightBorder;					/** Represents boundaries of chart's time range. */
	QString chartTitle;									/** Title displaying above charts. */
	bool dayOrNight = true;								/** Variable used to check what mode to draw charts: dark or light. */

	// Parser attributes
	QString configPath;									/** Path to parameters configuration file. */
	std::unique_ptr<Param[]> parsedParameters;			/** Unique pointer keeping information about all parameters in configuration. */
	std::vector<QString> seriesSets;					/** Vector containing information about all possible series sets in configuration. */

	// Query attributes
	QSqlDatabase db;									/** Object handling a connection to database. */
	int numOfParams;									/** Number of params in configuration. */
	typedef enum {										/** Enum used to determine which query to send to database. */
		DEFAULT,
		LEFT,
		RIGHT } TypeOfQuery;

	// Data sets attributes
	int numberOfQueryRows;								/** Number of rows returned by database query. */
	QLineSeries *series_[5];							/** Pointer containing data series to draw on chart. */
	QString legend[5] = {"", "", "", "", ""};			/** Table containing information about legend below charts. */
	std::vector<std::unique_ptr<int[]>> controlVector;	/** Vector whereby are calculated properties on right panel.  */
	std::unique_ptr<int[]> dataSeriesValue;				/** Unique pointer keeping particular value samples of data series. */
	std::unique_ptr<QDateTime[]> dataSeriesTimestamp;	/** Unique pointer keeping particular timestamp samples of data series. */

	//Functions
	/** Creating expandable menu on toolbar */
	void createMenus();
	/** Parsing configuration file and counting how many parameters it has. */
	int CountParamsInConfig();
	/** Parsing configuration and saving all available information about parameters.*/
	void ParseConfig();
	/** Getting portion of data series to be currently displayed on chart widget. */
	void GetDataSeries(TypeOfQuery);
	/** Preparing charts including:
	 *		setting charts title
	 *		setting displayed range at date and value axis
	 *		setting axes
	 *		setting legend
	 *		setting formatting */
	void SetupAndDrawCharts();
};

#endif // MAINWINDOW_H
