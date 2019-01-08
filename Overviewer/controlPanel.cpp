/*
 * =====================================================================================
 *
 *       Filename:  controlPanel.cpp
 *
 *         Author:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <QFrame>
#include "controlPanel.h"

ControlPanel::ControlPanel(QChart *left_chart, QString *legend_names) {
	this->setMinimumSize(minimumWidth, minimumHeight);
	chart = left_chart;
	QVBoxLayout *mainLayout = new QVBoxLayout;
	CreateLegend(legend_names);
	CreatePropertiesBox();

	dataSetChooser = new QComboBox();
	//TODO: dodawanie do combo boxa zestawow
	dataSetChooser->addItem("Temperatures");
	nightDay = new QPushButton(tr("Set night view"));

	mainLayout->addWidget(legend);
	mainLayout->addWidget(dataSetChooser);
	mainLayout->addWidget(seriesProperties);
	mainLayout->addWidget(nightDay);

	mainLayout->setSpacing(20);
	setLayout(mainLayout);

	connect(nightDay, SIGNAL(clicked()), this, SLOT(dayNightFunc()));
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
			legendLabels[i] = new QLabel(QString::number(i+1) + ". " + legend_names[i]);
			layout->addWidget(legendLabels[i]);
		} else {
			legendLabels[i] = new QLabel(tr(""));
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

