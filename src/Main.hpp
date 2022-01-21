#ifndef MAIN_HPP
#define MAIN_HPP

#include <stdint.h>
#include <string>

uint32_t GetServerTick();
std::string GetMap();
void DebugData(const char *fmt, ...);

#endif
