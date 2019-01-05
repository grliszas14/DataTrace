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

class ControlPanel : public QFrame
{
	Q_OBJECT

public:
	ControlPanel();

private:
	void CreateLegend();
	void CreatePropertiesBox();

	enum { ParamsInLegend = 5, NumOfProperties = 4 };
	const int minimumWidth = 200;
	const int minimumHeight = 500;

	QGroupBox *outsideGroupBox;
	QGroupBox *legend;
	QComboBox *dataSetChooser;
	QGroupBox *seriesProperties;
	QPushButton *nightDay;
	QLabel *legendLabels[ParamsInLegend];
	QLabel *propertiesLabels[NumOfProperties];

};

#endif //CONTROLPANEL_H
