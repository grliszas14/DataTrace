/*
 * =====================================================================================
 *
 *       Filename:  param.h
 *
 *    Description:  Class defining attributes of parameter used in overviewer after parsing config file.
 *
 *         Author:  Grzegorz Wojciechowski
 *
 * =====================================================================================
 */
#include <string>
#include <stdlib.h>

class Param
{
public:
	Param(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5, std::string s6);
	Param();

	std::string name;
	std::string short_name;
	std::string unit;
	std::string seriesSet;
	std::string minValue;
	std::string maxValue;
};
