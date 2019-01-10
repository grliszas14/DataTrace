/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.cpp
 *
 *    Description:  App made for overviewing data gathered by DataTrace system.
 *
 *        Authors:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#include <iostream>
#include <stdlib.h>
#include <QtWidgets>
#include <QtGui>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QList>
#include <algorithm>
#include "mainWindow.h"
#include <QtCharts>
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../rapidxml-1.13/rapidxml_utils.hpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QWidget *centralWidget = new QWidget();
	centralWidget->setMinimumSize(QSize(1200,600));
	chart = new QChart();
	chart->setTheme(QChart::ChartThemeLight);
	chartView = new QChartView(chart);
	maxToDisplay = -32768;
	minToDisplay = 32768;

	// Parse config file
#ifdef _WIN32
    configPath = QDir::currentPath() + "/../DataTrace-master/Config/config.xml";
#else
	configPath = QDir::currentPath() + "/Config/config.xml";
#endif
	numOfParams = CountParamsInConfig();
	parsedParameters = std::make_unique<Param[]>(numOfParams);
	ParseConfig();

	// Get default data and draw charts
	// Default behaviour is to gather 10 last probes of 1st param in config
	db = QSqlDatabase::addDatabase("QMYSQL");
	GetDataSeries(DEFAULT);
	SetupAndDrawCharts();

	// Make control panel
	rightPanel_ = new ControlPanel(chart, legend, &controlVector, &seriesSets);

	// Make scrolling arrows
	QHBoxLayout *arrows = new QHBoxLayout();
	scrollLeft_ = new QPushButton("<-");
	scrollRight_ = new QPushButton("->");
	arrows->addWidget(scrollLeft_);
	arrows->addWidget(scrollRight_);

	// Group charts and arrows
	QVBoxLayout *chartPanel = new QVBoxLayout();
	chartPanel->addWidget(chartView);
	chartPanel->addLayout(arrows);

	// Build whole layout
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addLayout(chartPanel);
	mainLayout->addWidget(rightPanel_);

	// Set all made things as central widget of window
	centralWidget->setLayout(mainLayout);
	setWindowTitle(tr("Overviewer"));
	setCentralWidget(centralWidget);

	connect(scrollLeft_, SIGNAL(clicked()), this, SLOT(ScrollLeftFunc()));
	connect(scrollRight_, SIGNAL(clicked()), this, SLOT(ScrollRightFunc()));
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

int MainWindow::CountParamsInConfig()
{
	int countParameters = 0;
	rapidxml::file<> xmlFile(configPath.toUtf8().constData());
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;
	doc.parse<0>(xmlFile.data());

	root_node = doc.first_node("parameters");
	for (rapidxml::xml_node<> * param_node = root_node->first_node("param"); param_node; param_node = param_node->next_sibling())
	{
		countParameters++;
	}

	return countParameters;
}

void MainWindow::ParseConfig() {
	rapidxml::file<> xmlFile(configPath.toUtf8().constData());
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;
	doc.parse<0>(xmlFile.data());

	root_node = doc.first_node("parameters");
	rapidxml::xml_node<> * param_node;
	int i;
	for (param_node = root_node->first_node("param"), i = 0;
			param_node;
			param_node = param_node->next_sibling(), ++i)
	{
		Param param = Param(param_node->first_attribute("name")->value(),
									param_node->first_attribute("short_name")->value(),
									param_node->first_attribute("unit")->value(),
									param_node->first_attribute("series_set")->value(),
									"",
									"");
		parsedParameters[i] = param;
		if (std::find(seriesSets.begin(), seriesSets.end(), QString::fromStdString(parsedParameters[i].seriesSet)) == seriesSets.end()) {
			seriesSets.push_back(QString::fromStdString(parsedParameters[i].seriesSet));
		}
	}

}

void MainWindow::GetDataSeries(TypeOfQuery toq) {
	// Connect to database
    db.setHostName("localhost");
    db.setDatabaseName("DataTrace");
    db.setUserName("grliszas14");
    db.setPassword("test");
	db.open();

	// Check first data_set and get data series
	std::string current_series_set = parsedParameters[0].seriesSet;
	chartTitle = QString::fromStdString(current_series_set);
	//TODO mechanizmy query, oddzielna funkcja
	int series_counter = 0;
	if (controlVector.size() > 0)
		controlVector.clear();
	switch(toq)
	{
		case LEFT:
		case RIGHT:
			chart->removeAllSeries();
			chart->removeAxis(axisX);
			chart->removeAxis(axisY);
			break;
		case DEFAULT:
			break;
	}
	for (int j = 0; j < numOfParams; ++j ) {
		if ( parsedParameters[j].seriesSet == current_series_set ) {
			QSqlQuery query;
			switch (toq)
			{
				case DEFAULT:
#ifdef _WIN32
					query.exec("SELECT * FROM (SELECT * FROM " + QString::fromStdString(parsedParameters[j].name).toLower() + " ORDER BY date DESC LIMIT 10) sub ORDER BY date ASC");
#else
					query.exec("SELECT * FROM (SELECT * FROM " + QString::fromStdString(parsedParameters[j].name) + " ORDER BY date DESC LIMIT 10) sub ORDER BY date ASC");
#endif
					break;
				case LEFT:
#ifdef _WIN32
					query.exec("SELECT * FROM (SELECT * FROM " + QString::fromStdString(parsedParameters[j].name).toLower() + " WHERE date < \"" + rightBorder.toString("yyyy-MM-dd hh:mm:ss") + "\" ORDER BY date DESC LIMIT 10) sub ORDER BY date ASC");
#else
					query.exec("SELECT * FROM (SELECT * FROM " + QString::fromStdString(parsedParameters[j].name) + " WHERE date < \"" + rightBorder.toString("yyyy-MM-dd hh:mm:ss") + "\" ORDER BY date DESC LIMIT 10) sub ORDER BY date ASC");
#endif
					break;
				case RIGHT:
#ifdef _WIN32
					query.exec("SELECT * FROM " + QString::fromStdString(parsedParameters[j].name).toLower() + " WHERE date > \"" + leftBorder.toString("yyyy-MM-dd hh:mm:ss") + "\" LIMIT 10");
#else
					query.exec("SELECT * FROM " + QString::fromStdString(parsedParameters[j].name) + " WHERE date > \"" + leftBorder.toString("yyyy-MM-dd hh:mm:ss") + "\" LIMIT 10");
#endif
					break;
			}
			legend[j] = QString::fromStdString(parsedParameters[j].short_name);
			numberOfQueryRows = query.size();

			dataSeriesValue = std::make_unique<int[]>(numberOfQueryRows);
			dataSeriesTimestamp = std::make_unique<QDateTime[]>(numberOfQueryRows);

			// Read query
			int i = 0;
			while (query.next()) {
				dataSeriesTimestamp[i] = query.value(0).toDateTime();
				dataSeriesValue[i] = query.value(1).toInt();

				// Calculate chart borders
				if ( dataSeriesValue[i] > maxToDisplay) maxToDisplay = dataSeriesValue[i];
				if ( dataSeriesValue[i] < minToDisplay) minToDisplay = dataSeriesValue[i];
				++i;
			}

			QList<QPointF> chartPoints;
			series_[series_counter] = new QLineSeries();
			series_[series_counter]->setName(QString::number(series_counter + 1));

			// Default behaviour: display 10 first probes
			for (int i = 0; i < numberOfQueryRows; ++i) {
				series_[series_counter]->append(QPointF(dataSeriesTimestamp[i].toMSecsSinceEpoch(), dataSeriesValue[i]));
			}
			controlVector.push_back(std::move(dataSeriesValue));
			chart->addSeries(series_[series_counter]);
			series_counter++;
		}
	}

	// Protection from the end of the base (left/right)
	switch(toq){
		case DEFAULT:
			rightBorder = dataSeriesTimestamp[numberOfQueryRows-1];
			leftBorder = dataSeriesTimestamp[0];
			break;
		case RIGHT:
			if( numberOfQueryRows == 10) {
				rightBorder = dataSeriesTimestamp[numberOfQueryRows-1];
				leftBorder = dataSeriesTimestamp[0];
			}
			break;
		case LEFT:
			if( numberOfQueryRows == 10) {
				rightBorder = dataSeriesTimestamp[numberOfQueryRows-1];
				leftBorder = dataSeriesTimestamp[0];
			}
			break;
	}

	db.close();
}

void MainWindow::SetupAndDrawCharts() {
	// Setup and draw charts
	chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTitle(chartTitle);
	axisX = new QDateTimeAxis;
	axisX->setRange(leftBorder, rightBorder);
	axisX->setTickCount(10);
	axisX->setFormat("hh:mm:ss");
	axisX->setTitleText("Date");
	chart->addAxis(axisX, Qt::AlignBottom);
	axisY = new QValueAxis;
	axisY->setLabelFormat("%i");
	axisY->setTickCount(3);
	axisY->setRange(minToDisplay, maxToDisplay);
	chart->addAxis(axisY, Qt::AlignLeft);
	for( int i = 0; i < 5; ++i) {
		series_[i]->attachAxis(axisX);
		series_[i]->attachAxis(axisY);
	}
    chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::ScrollLeftFunc() {
	GetDataSeries(LEFT);
	SetupAndDrawCharts();
}

void MainWindow::ScrollRightFunc() {
	GetDataSeries(RIGHT);
	SetupAndDrawCharts();
}

MainWindow::~MainWindow()
{
	delete fileMenu_;
	delete editMenu_;
	delete helpMenu_;
	delete graphsFrame_;
	delete rightPanel_;
}

