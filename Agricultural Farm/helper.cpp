#include "_time.h"
#include "helper.h"

Helper::Helper()
{

}

_Time Helper::TimeFromHours(float hours)
{
	float h = static_cast< int >(hours);

	float minutes = (hours - h) * 60;

	float m = static_cast< int >(minutes);

	float seconds = (minutes - m) * 60;

	float s = static_cast< int >(seconds);

	float msecs = (seconds - s) * 1000;

	float ms = static_cast< int >(msecs);

	_Time t = _Time(h, m, s, ms);

	// Time test = Time(h, m, s, ms);

	// cout << "h : " << test.h << endl;

	return t;
}

_Time Helper::TimeFromDays(float days)
{
	float h = days * 24;

	return TimeFromHours(h);
}
