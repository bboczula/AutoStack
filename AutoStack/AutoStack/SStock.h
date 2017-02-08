#pragma once

#include "SDate.h"

struct SStock
{
	SDate date;
	double open;
	double max;
	double min;
	double close;
	double volume;
};