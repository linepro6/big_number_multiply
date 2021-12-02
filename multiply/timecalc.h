#pragma once
#include <time.h>
#include <stdint.h>
/// Convert seconds to milliseconds
#define SEC_TO_MS(sec) ((sec)*1000)
/// Convert seconds to microseconds
#define SEC_TO_US(sec) ((sec)*1000000)
/// Convert seconds to nanoseconds
#define SEC_TO_NS(sec) ((sec)*1000000000)

/// Convert nanoseconds to seconds
#define NS_TO_SEC(ns)   ((ns)/1000000000)
/// Convert nanoseconds to milliseconds
#define NS_TO_MS(ns)    ((ns)/1000000)
/// Convert nanoseconds to microseconds
#define NS_TO_US(ns)    ((ns)/1000)

/// Get a time stamp in milliseconds.
uint64_t millis()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	uint64_t ms = SEC_TO_MS((uint64_t)ts.tv_sec) + NS_TO_MS((uint64_t)ts.tv_nsec);
	return ms;
}

/// Get a time stamp in microseconds.
uint64_t micros()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	uint64_t us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec);
	return us;
}

/// Get a time stamp in nanoseconds.
uint64_t nanos()
{
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	uint64_t ns = SEC_TO_NS((uint64_t)ts.tv_sec) + (uint64_t)ts.tv_nsec;
	return ns;
}

void printNsTimeDelta(uint64_t ns1, uint64_t ns2)
{
	uint64_t ns = ns2 - ns1;
	uint64_t us = NS_TO_US(ns);
	if (!us)
	{
		printf("%llu ns", ns);
		return;
	}
	uint64_t ms = NS_TO_MS(ns);
	if (!ms)
	{
		printf("%llu.%.3llu us", us, ns % 1000);
		return;
	}
	uint64_t sec = NS_TO_SEC(ns);
	if (!sec)
	{
		printf("%llu.%.3llu ms", ms, us % 1000);
		return;
	}
	uint64_t min = sec / 60;
	if (!min)
	{
		printf("%llu.%.3llu s", sec, ms % 1000);
		return;
	}
	printf("%llu:%.2llu", min, sec % 60);
}