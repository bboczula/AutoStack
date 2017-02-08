#pragma once

#include <string>

struct SDate
{
	unsigned int day;
	unsigned int month;
	unsigned int year;
	std::string toString()
	{
		return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
	}
};