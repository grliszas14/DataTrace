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
#include "../LibraryDT/configParser.cpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	QWidget *centralWidget = new QWidget();
	centralWidget->setFixedSize(QSize(1200,600));

	createMenus();

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("DataTrace");
    db.setUserName("grliszas14");
    db.setPassword("test");
    bool ok = db.open();

	if (ok == true) {
		qInfo() << "OK";
	} else {
		qInfo() << "NOT OK";
	}

	QSqlQuery query;
	query.exec("SELECT * FROM Polska_Gdansk_Temperatura_zewnetrzna");

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

	for (int i = 1; i < 10; ++i) {
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

MainWindow::~MainWindow()
{
	delete fileMenu_;
	delete editMenu_;
	delete helpMenu_;
	delete graphsFrame_;
	delete rightPanel_;
}

