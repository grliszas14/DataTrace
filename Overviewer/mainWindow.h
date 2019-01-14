/*
 * =====================================================================================
 *
 *		     @file:  mainWindow.h
 *
 *			@brief:  App made for overviewing data gathered by DataTrace system.
 *
 *		   @author:  Grzegorz Wojciechowski
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

/**	@brief Class representing main window of the overviewer application. */
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
	/** Constructs a context menu event object. */
	void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
	/** @fn ScrollLeftFunc();
	 * @brief Function connected to arrow below charts widget. Scrolls charts to the left. */
	void ScrollLeftFunc();
	/** @fn ScrollRightFunc();
	 * @brief Function connected to arrow below charts widget. Scrolls charts to the right. */
	void ScrollRightFunc();

private:
	// GUI attributes
	/** Expandable file menu from toolbar. */
	QMenu* fileMenu_;
	/** Expandable edit menu from toolbar. */
	QMenu* editMenu_;
	/** Expandable help menu from toolbar. */
	QMenu* helpMenu_;
	/** Arrow allowing scrolling charts. */
	QPushButton* scrollLeft_;
	/** Arrow allowing scrolling charts. */
	QPushButton* scrollRight_;
	/** Panel on the right side of app. */
	ControlPanel* rightPanel_;
	/** Widget managing graphical representation of the chart's series, legend and axes. */
	QChart* chart;
	/** Standalone widget that can display charts. */
	QChartView *chartView;
	/** Represents time axis on chart. */
	QDateTimeAxis *axisX;
	/** Represents value axis on the chart. */
	QValueAxis *axisY;
	/** Represents boundaries of chart's values. */
	int maxToDisplay;
	/** Represents boundaries of chart's values. */
	int minToDisplay;
	/** Represents boundaries of chart's time range. */
	QDateTime leftBorder;
	/** Represents boundaries of chart's time range. */
    QDateTime rightBorder;
	/** Title displaying above charts. */
	QString chartTitle;
	/** Variable used to check what mode to draw charts: dark or light. */
	bool dayOrNight = true;

	// Parser attributes
	/** Path to parameters configuration file. */
	QString configPath;
	/** Unique pointer keeping information about all parameters in configuration. */
	std::unique_ptr<Param[]> parsedParameters;
	/** Vector containing information about all possible series sets in configuration. */
	std::vector<QString> seriesSets;

	// Query attributes
	/** Object handling a connection to database. */
	QSqlDatabase db;
	/** Number of params in configuration. */
	int numOfParams;
	/** Enum used to determine which query to send to database. */
	typedef enum {
	/** Default query on start of a program looking for 10 last samples in database.  */
		DEFAULT,
	/** Query searching samples to the left side */
		LEFT,
	/** Query searching samples to the left side */
		RIGHT } TypeOfQuery;

	// Data sets attributes
	/** Number of rows returned by database query. */
	int numberOfQueryRows;
	/** Pointer containing data series to draw on chart. */
	QLineSeries *series_[5];
	/** Table containing information about legend below charts. */
	QString legend[5] = {"", "", "", "", ""};
	/** Vector whereby are calculated properties on right panel.  */
	std::vector<std::unique_ptr<int[]>> controlVector;
	/** Unique pointer keeping particular value samples of data series. */
	std::unique_ptr<int[]> dataSeriesValue;
	/** Unique pointer keeping particular timestamp samples of data series. */
	std::unique_ptr<QDateTime[]> dataSeriesTimestamp;

	//Functions
	/** @brief Creating expandable menu on toolbar */
	void createMenus();
	/** @brief Parsing configuration file and counting how many parameters it has. */
	int CountParamsInConfig();
	/** @brief Parsing configuration and saving all available information about parameters.*/
	void ParseConfig();
	/** @brief Getting portion of data series to be currently displayed on chart widget. */
	void GetDataSeries(TypeOfQuery);
	/** @brief Preparing charts including:
	 *		setting charts title
	 *		setting displayed range at date and value axis
	 *		setting axes
	 *		setting legend
	 *		setting formatting */
	void SetupAndDrawCharts();
};

#endif // MAINWINDOW_H
