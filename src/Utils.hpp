#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
	std::string ssprintf(const char *fmt, ...);
	std::string FormatTime(uint32_t ticks);
}

#endif
