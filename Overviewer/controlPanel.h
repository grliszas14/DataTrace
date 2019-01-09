/*
 * =====================================================================================
 *
 *       Filename:  controlPanel.h
 *
 *         Author:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */


#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QFrame>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QtCharts>

class ControlPanel : public QFrame
{
	Q_OBJECT

public:
	ControlPanel(QChart *chart, QString *legend_names);

private slots:
	void dayNightFunc();

private:
	void CreateLegend(QString *legend_names);
	void CreatePropertiesBox();

	enum { ParamsInLegend = 5, NumOfProperties = 4 };
	const int minimumWidth = 200;
	const int minimumHeight = 500;
	bool day_or_night = true;

	QChart *chart;
	QGroupBox *outsideGroupBox;
	QGroupBox *legend;
	QComboBox *dataSetChooser;
	QGroupBox *seriesProperties;
	QPushButton *nightDay;
	QPushButton *legendLabels[ParamsInLegend];
	QLabel *propertiesLabels[NumOfProperties];

};

#endif //CONTROLPANEL_H
