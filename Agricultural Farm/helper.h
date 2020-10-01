#ifndef __HELPER_H
#define __HELPER_H

#include "_time.h"

class Helper
{
	public:
		Helper();

		_Time TimeFromHours(float hours);
		_Time TimeFromDays(float days);
};

#endif
