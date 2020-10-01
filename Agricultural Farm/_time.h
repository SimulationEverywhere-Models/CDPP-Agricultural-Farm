#ifndef ___TIME_H
#define ___TIME_H

#include "time.h"

class _Time
{
	public:
		int h;
		int m;
		int s;
		int ms;

		_Time();
		_Time(int h, int m, int s, int ms);
		~_Time();
};

#endif
