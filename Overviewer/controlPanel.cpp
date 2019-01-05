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

ControlPanel::ControlPanel() {
	this->setMinimumSize(minimumWidth, minimumHeight);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	CreateLegend();
	CreatePropertiesBox();

	dataSetChooser = new QComboBox();
	//TODO: dodawanie do combo boxa zestawow
	dataSetChooser->addItem("Temperatures");
	nightDay = new QPushButton(tr("Day/Night"));

	mainLayout->addWidget(legend);
	mainLayout->addWidget(dataSetChooser);
	mainLayout->addWidget(seriesProperties);
	mainLayout->addWidget(nightDay);

	mainLayout->setSpacing(20);
	setLayout(mainLayout);
}

void ControlPanel::CreateLegend() {
	legend = new QGroupBox(tr("Legend"));
	QVBoxLayout *layout = new QVBoxLayout;

	for (int i = 0; i < ParamsInLegend; ++i) {
		legendLabels[i] = new QLabel(tr("Parameter name"));
		layout->addWidget(legendLabels[i]);
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

