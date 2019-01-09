/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.cpp
 *
 *    Description:  App made for overview data gathered by DataTrace system
 *
 *        Authors:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#include <iostream>
#include <stdlib.h>
#include <QtWidgets>
#include <QtGui>
#include <QtSql>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QList>
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

	// Parse config file
	int numOfParams = CountParamsInConfig();
	qDebug() << numOfParams;
	parsedParameters = std::make_unique<Param[]>(numOfParams);
	ParseConfig();

	// Connect to database
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("DataTrace");
    db.setUserName("grliszas14");
    db.setPassword("test");
    bool ok = db.open();

	if (ok == true) {
		qInfo() << "DATABASE CONNECTION OK";
	} else {
		qInfo() << "DATABASE CONNECTION NOT OK";
	}

	// Check first data_set and get data series
	std::string current_series_set = parsedParameters[0].seriesSet;
	//TODO mechanizmy query, oddzielna funkcja
	int series_counter = 0;
	for (int j = 0; j < numOfParams; ++j ) {
		if ( parsedParameters[j].seriesSet == current_series_set ) {
			QSqlQuery query;
			query.exec("SELECT * FROM " + QString::fromStdString(parsedParameters[j].name));
			legend[j] = QString::fromStdString(parsedParameters[j].short_name);
			numberOfQueryRows = query.size();

			dataSeriesValue = std::make_unique<int[]>(numberOfQueryRows);
			dataSeriesTimestamp = std::make_unique<QDateTime[]>(numberOfQueryRows);
			int i = 0;
			while (query.next()) {
				dataSeriesTimestamp[i] = query.value(0).toDateTime();
				dataSeriesValue[i] = query.value(1).toInt();
				++i;
			}

			QList<QPointF> chartPoints;
			series_[series_counter] = new QLineSeries();
			series_[series_counter]->setName(QString::number(series_counter + 1));

			// Default behaviour: display 10 first probes
			for (int i = 0; i < 10; ++i) {
				series_[series_counter]->append(QPointF(dataSeriesTimestamp[i].toMSecsSinceEpoch(), dataSeriesValue[i]));
			}
			chart->addSeries(series_[series_counter]);
			series_counter++;
		}
	}

	// Setup and draw charts
	chart->setTheme(QChart::ChartThemeLight);
	chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTitle("Temperatures"); //TODO pobrac z configa
	// Ustawiac customowo w zaleznosci od rozbieznosci danych
	QDateTimeAxis *axisX = new QDateTimeAxis;
	axisX->setRange(dataSeriesTimestamp[1], dataSeriesTimestamp[9]);
	axisX->setTickCount(10);
	axisX->setFormat("hh:mm:ss");
	axisX->setTitleText("Date");
	chart->addAxis(axisX, Qt::AlignBottom);
	QValueAxis *axisY = new QValueAxis;
	axisY->setLabelFormat("%i");
	axisY->setTickCount(3);
	axisY->setRange(-4, 4);
	chart->addAxis(axisY, Qt::AlignLeft);
	for( int i = 0; i < 5; ++i) {
		series_[i]->attachAxis(axisX);
		series_[i]->attachAxis(axisY);
	}
	QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

	// Make control panel
	rightPanel_ = new ControlPanel(chart, legend);
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget(chartView);
	mainLayout->addWidget(rightPanel_);

	// Set all made things as central widget of window
	centralWidget->setLayout(mainLayout);
	setWindowTitle(tr("Overviewer"));
	setCentralWidget(centralWidget);
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
	rapidxml::file<> xmlFile("/home/grliszas14/zpr/DataTrace/Config/config.xml"); //TODO: tu nie moze byc full path
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
	rapidxml::file<> xmlFile("/home/grliszas14/zpr/DataTrace/Config/config.xml"); //TODO: tu nie moze byc full path
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
		std::cout << param.name << std::endl;
	}

}

MainWindow::~MainWindow()
{
	delete fileMenu_;
	delete editMenu_;
	delete helpMenu_;
	delete graphsFrame_;
	delete rightPanel_;
}

