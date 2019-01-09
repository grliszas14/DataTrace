/*
 * =====================================================================================
 *
 *       Filename:  controlPanel.cpp
 *
 *	  Description:  Class containing right panel of application.
 *					Displays legend with parameters' names, parameters' properties
 *					if clicked on legend, chooser of data series set and button
 *					enabling/disabling night mode.
 *
 *         Author:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <QFrame>
#include <cmath>
#include "controlPanel.h"

ControlPanel::ControlPanel(QChart *left_chart, QString *legend_names, std::vector<std::unique_ptr<int[]>> *controlVectorFromMainWidget, std::vector<QString> *seriesSetsFromMainWidget) {
	this->setMinimumSize(minimumWidth, minimumHeight);
	chart = left_chart;
	QVBoxLayout *mainLayout = new QVBoxLayout;
	controlVector = controlVectorFromMainWidget;
	seriesSets = seriesSetsFromMainWidget;
	CreateLegend(legend_names);
	CreatePropertiesBox();

	dataSetChooser = new QComboBox();
	for (unsigned int i = 0; i < seriesSets->size(); ++i)
		dataSetChooser->addItem(seriesSets->at(i));
	nightDay = new QPushButton(tr("Set night view"));

	mainLayout->addWidget(legend);
	mainLayout->addWidget(dataSetChooser);
	mainLayout->addWidget(seriesProperties);
	mainLayout->addWidget(nightDay);

	mainLayout->setSpacing(20);
	setLayout(mainLayout);

	// Using signal mapper to pass parameter into slot function
	// TODO zmienic na lambdy!!
	QSignalMapper* signalMapper = new QSignalMapper (this);
	connect(legendLabels[0], SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(legendLabels[1], SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(legendLabels[2], SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(legendLabels[3], SIGNAL(clicked()), signalMapper, SLOT(map()));
	connect(legendLabels[4], SIGNAL(clicked()), signalMapper, SLOT(map()));

	signalMapper->setMapping(legendLabels[0],0);
	signalMapper->setMapping(legendLabels[1],1);
	signalMapper->setMapping(legendLabels[2],2);
	signalMapper->setMapping(legendLabels[3],3);
	signalMapper->setMapping(legendLabels[4],4);

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(showProperties(int)));
	connect(nightDay, SIGNAL(clicked()), this, SLOT(dayNightFunc()));
}

void ControlPanel::showProperties(int series) {
	double maxOfSeries = CalculateMax(series);
	double minOfSeries = CalculateMin(series);
	double meanOfSeries = CalculateMean(series);
	double deviationOfSeries = CalculateDeviation(series);

	propertiesLabels[0]->setText(QString("Max: ") + QString::number(maxOfSeries));
	propertiesLabels[1]->setText(QString("Min: ") + QString::number(minOfSeries));
	propertiesLabels[2]->setText(QString("Mean: ") + QString::number(meanOfSeries));
	propertiesLabels[3]->setText(QString("Deviation: ") + QString::number(deviationOfSeries));
}

double ControlPanel::CalculateMin(int series) {
	double minValue = 0;
	// TODO docelowo index ponizej
	minValue = controlVector->at(series)[0];
	for( int i = 0; i < NUM_OF_PROBES_TO_CALC; ++i) {
		if( controlVector->at(series)[i] < minValue) minValue = controlVector->at(series)[i];
	}
	return minValue;
}

double ControlPanel::CalculateMax(int series) {
	double maxValue = 0;
	// TODO docelowo index ponizej
	maxValue = controlVector->at(series)[0];
	for( int i = 0; i < NUM_OF_PROBES_TO_CALC; ++i) {
		if( controlVector->at(series)[i] > maxValue) maxValue = controlVector->at(series)[i];
	}
	return maxValue;
}

double ControlPanel::CalculateMean(int series) {
	int sum = 0;
	double mean = 0;
	// TODO docelowo index ponizej
	for( int i = 0; i < NUM_OF_PROBES_TO_CALC; ++i)
		sum += controlVector->at(series)[i];
	mean = sum / NUM_OF_PROBES_TO_CALC;
	return mean;
}

double ControlPanel::CalculateDeviation(int series) {
	int sum = 0;
	double deviation = 0;
	double mean = CalculateMean(series);
	std::vector<int> tmpToDeviation;
	// TODO docelowo index ponizej
	for( int i = 0; i < NUM_OF_PROBES_TO_CALC; ++i) {
		double calculation = pow(controlVector->at(series)[i] - mean, 2);
		tmpToDeviation.push_back(calculation);
	}
	for ( int n : tmpToDeviation)
		sum += n;
	deviation = sqrt(sum / NUM_OF_PROBES_TO_CALC);

	return deviation;
}

void ControlPanel::dayNightFunc() {
	if (day_or_night == true) {
		nightDay->setText("Set day view");
		day_or_night = false;
		chart->setTheme(QChart::ChartThemeDark);
	} else {
		nightDay->setText("Set night view");
		day_or_night = true;
		chart->setTheme(QChart::ChartThemeLight);
	}
}

void ControlPanel::CreateLegend(QString *legend_names) {
	legend = new QGroupBox(tr("Legend"));
	QVBoxLayout *layout = new QVBoxLayout;

	for (int i = 0; i < ParamsInLegend; ++i) {
		if (legend_names[i] != "") {
			legendLabels[i] = new QPushButton(QString::number(i+1) + ". " + legend_names[i]);
			legendLabels[i]->setFlat(true);
			layout->addWidget(legendLabels[i]);
		} else {
			legendLabels[i] = new QPushButton(tr(""));
			legendLabels[i]->setFlat(true);
			layout->addWidget(legendLabels[i]);
		}
	}
	legend->setLayout(layout);
}

void ControlPanel::CreatePropertiesBox() {
	seriesProperties = new QGroupBox(tr("Param properties"));
	QVBoxLayout *propertiesLayout = new QVBoxLayout;

	propertiesLabels[0] = new QLabel(tr("Max:"));
	propertiesLabels[1] = new QLabel(tr("Min:"));
	propertiesLabels[2] = new QLabel(tr("Mean:"));
	propertiesLabels[3] = new QLabel(tr("Deviation:"));

	for (int i = 0; i < NumOfProperties; ++i) {
		propertiesLayout->addWidget(propertiesLabels[i]);
	}
	seriesProperties->setLayout(propertiesLayout);
}

