#include "Utils.hpp"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "Timer.hpp"

std::string Utils::ssprintf(const char *fmt, ...)
{
    va_list ap1, ap2;
    va_start(ap1, fmt);
    va_copy(ap2, ap1);
    size_t sz = vsnprintf(NULL, 0, fmt, ap1) + 1;
    va_end(ap1);
    char *buf = (char *)malloc(sz);
    vsnprintf(buf, sz, fmt, ap2);
    va_end(ap2);
		std::string str(buf);
    free(buf);
    return str;
}

std::string Utils::FormatTime(uint32_t ticks) {
	uint32_t total = ((float)ticks / IPT) * 1000;

	int ms = total % 1000;
	total /= 1000;

	int secs = total % 60;
	total /= 60;

	int mins = total % 60;
	total /= 60;

	int hrs = total;

	if (hrs) {
		return Utils::ssprintf("%d:%02d:%02d.%03d", hrs, mins, secs, ms);
	}

	if (mins) {
		return Utils::ssprintf("%d:%02d.%03d", mins, secs, ms);
	}

	return Utils::ssprintf("%d.%03d", secs, ms);
}
