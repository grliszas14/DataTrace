/*
 * =====================================================================================
 *
 *       Filename:  mainWindow.cpp
 *
 *    Description:
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
#include "chartsFrame.h"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../rapidxml-1.13/rapidxml_utils.hpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QWidget *centralWidget = new QWidget();
	centralWidget->setMinimumSize(QSize(1200,600));

	createMenus();

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

	//TODO mechanizmy query

	QSqlQuery query;
	query.exec("SELECT * FROM " + QString::fromStdString(parsedParameters[0].name));

	numberOfQueryRows = query.size();
	qDebug() << numberOfQueryRows;

	dataSeriesValue = std::make_unique<int[]>(numberOfQueryRows);
	dataSeriesTimestamp = std::make_unique<QDateTime[]>(numberOfQueryRows);
	int i = 0;
	while (query.next()) {
		dataSeriesTimestamp[i] = query.value(0).toDateTime();
		dataSeriesValue[i] = query.value(1).toInt();
		++i;
	}

	QList<QPointF> chartPoints;
	QLineSeries *series_ = new QLineSeries();

	// Default behaviour: display 10 first probes
	for (int i = 0; i < 10; ++i) {
		qDebug() << dataSeriesTimestamp[i].toMSecsSinceEpoch() << dataSeriesValue[i];
		series_->append(QPointF(dataSeriesTimestamp[i].toMSecsSinceEpoch(), dataSeriesValue[i]));
	}

    QPen pen(0xffff00);
    pen.setWidth(6);
	series_->setPen(pen);
	QChart *chart = new QChart();
	chart->setTheme(QChart::ChartThemeDark);
    chart->addSeries(series_);
	chart->legend()->hide();
    chart->setTitle("Temperatury dużych miast w Polsce");
	// Ustawiac customowo w zaleznosci od rozbieznosci danych
	QDateTimeAxis *axisX = new QDateTimeAxis;
	axisX->setRange(dataSeriesTimestamp[1], dataSeriesTimestamp[9]);
	axisX->setTickCount(10);
	axisX->setFormat("hh:mm:ss");
	axisX->setTitleText("Data");
	chart->addAxis(axisX, Qt::AlignBottom);
	series_->attachAxis(axisX);
	QValueAxis *axisY = new QValueAxis;
	axisY->setLabelFormat("%i");
	axisY->setTickCount(3);
	axisY->setTitleText("Temperatury");
	axisY->setRange(0, 4);
	chart->addAxis(axisY, Qt::AlignLeft);
	series_->attachAxis(axisY);
	QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

	// Make control panel
	rightPanel_ = new ControlPanel();
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget(chartView);
	mainLayout->addWidget(rightPanel_);

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
									"",
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

