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

/** @brief Class representing parameter and its attributes */
class Param
{
public:
	/** Initializing constructor. */
	Param(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5, std::string s6);
	/** Default constructor. */
	Param();

	/** Name of the parameter. */
	std::string name;
	/** Short name of the parameter, displayed in legend on the right side of app. */
	std::string short_name;
	/** Unit of the parameter. */
	std::string unit;
	/** Set to which parameter will be signed to. */
	std::string seriesSet;
	/** Minimum value for custom chart scalling. */
	std::string minValue;
	/** Maximum value for custom chart scalling. */
	std::string maxValue;
};
