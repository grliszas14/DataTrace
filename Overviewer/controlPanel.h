/*
 * =====================================================================================
 *
 *        Filename:  controlPanel.h
 *
 *			@brief: Class containing right panel of application.
 *
 *					Displays legend with parameters' names, parameters' properties
 *					if clicked on legend, chooser of data series set and button
 *					enabling/disabling night mode.
 *
 *         @author:  Grzegorz Wojciechowski
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

/** @brief Class making right panel of application: displaying legend, chooser with data sets, parameter properties and button with day/night mode. */
class ControlPanel : public QFrame
{
	Q_OBJECT

public:
	/** Default constructor.
	 * @param chart			- widget that can display charts.
	 * @param paramNames	- parameters names currently displayed on charts. Used to create legend on the right side of app.
	 * @param controlVector - vector containing values of data series samples. Used to calculate parameter properties.
	 * @param seriesSets	- vector containing data series sets names.  */
	ControlPanel(QChart *chart, QString *paramNames, std::vector<std::unique_ptr<int[]>> *controlVector, std::vector<QString> *seriesSets);

private slots:
	/** Connected to nightDay button. Changing displaying mode: dark/light. */
	void dayNightFunc();
	/** Connected to legend labels. If label clicked calculates properties of currently displayed series data. */
	void showProperties(int);

private:
	/** Creating legend box on the upper right side of app. */
	void CreateLegend(QString *paramNames);
	/** Creating box displaying properties of current data series of choosen parameter. */
	void CreatePropertiesBox();
	/** Calculates minimal value of currently displaying data of chosen param. */
	double CalculateMin(int series);
	/** Calculates maximum value of currently displaying data of chosen param. */
	double CalculateMax(int series);
	/** Calculates mean value of currently displaying data of chosen param. */
	double CalculateMean(int series);
	/** Calculates deviation of value of currently displaying data of chosen param. */
	double CalculateDeviation(int series);

	/** Maximum number of parameters in legend, number of all properties of parameter. */
	enum {
	/** Maximum number of parameters in legend. */
		ParamsInLegend = 5,
	/** Number of all properties of parameter. */
		NumOfProperties = 4 };
	/** Maximum width of right panel of the app.*/
	const int minimumWidth = 200;
	/** Minimum height of right panel of the app. */
	const int minimumHeight = 500;
	/** Number of probes to take into account in calculating properties. */
	const int NUM_OF_PROBES_TO_CALC = 10;
	/** Variable deciding which mode to draw charts: dark/light. */
	bool day_or_night = true;
	/** Vector containing samples used to calculate parameter properties. */
	std::vector<std::unique_ptr<int[]>> *controlVector;
	/** Vector containing names of all availabe series sets. Displayed in dataSetChooser. */
	std::vector<QString> *seriesSets;

	/** Widget managing graphical representation of the chart's series, legend and axes. */
	QChart *chart;
	/** Box grouping all widgets in control panel. */
	QGroupBox *outsideGroupBox;
	/** Box grouping names of parameters. */
	QGroupBox *legend;
	/** Chooser of currently displayed data set. */
	QComboBox *dataSetChooser;
	/** Box grouping properties of parameter. */
	QGroupBox *seriesProperties;
	/** Button changing display mode: dark/light. */
	QPushButton *nightDay;
	/** Labels (flat buttons exactly) with parameter name on it. */
	QPushButton *legendLabels[ParamsInLegend];
	/** Labels with calculated properties of parameter. */
	QLabel *propertiesLabels[NumOfProperties];
};

#endif //CONTROLPANEL_H
