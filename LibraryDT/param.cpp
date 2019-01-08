/*
 * =====================================================================================
 *
 *       Filename:  param.cpp
 *
 *         Author:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string>
#include "param.h"

Param::Param(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5, std::string s6) {
	name = s1;
	short_name = s2;
	unit = s3;
	seriesSet = s4;
	minValue = s5;
	maxValue= s6;
}

Param::Param() {
	name = "";
	short_name = "";
	unit = "";
	seriesSet = "";
	minValue = "";
	maxValue= "";

}
