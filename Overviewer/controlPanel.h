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
#include <vector>
#include <memory>

class ControlPanel : public QFrame
{
	Q_OBJECT

public:
	ControlPanel(QChart *chart, QString *legend_names, std::vector<std::unique_ptr<int[]>> *controlVector);

private slots:
	void dayNightFunc();
	void showProperties(int);

private:
	void CreateLegend(QString *legend_names);
	void CreatePropertiesBox();
	double CalculateMin(int series);
	double CalculateMax(int series);
	double CalculateMean(int series);
	double CalculateDeviation(int series);

	enum { ParamsInLegend = 5, NumOfProperties = 4 };
	const int minimumWidth = 200;
	const int minimumHeight = 500;
	const int NUM_OF_PROBES_TO_CALC = 10;
	bool day_or_night = true;
	std::vector<std::unique_ptr<int[]>> *controlVector;

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
