#include "Timer.hpp"
#include "Main.hpp"
#include "Rift.hpp"
#include "Utils.hpp"

#include <vector>
#include <string>

static uint32_t g_saved;
static uint32_t g_base;
static bool g_running;
static bool g_paused;

static std::vector<std::string> g_visitedMaps;

void Timer::Start(uint32_t offset) {
	g_saved = offset;
	g_base = GetServerTick();
	g_running = true;
	g_paused = false;
	Rift::Start(offset);
	g_visitedMaps.clear();
	g_visitedMaps.push_back(GetMap());
}

void Timer::Stop() {
	if (g_running) {
		g_saved += GetServerTick() - g_base;
		g_running = false;
		g_paused = false;
		Rift::Split(Timer::GetTicks());
	}
}

void Timer::Reset(uint32_t offset) {
	if (g_running) {
		g_running = false;
		g_paused = false;
		g_saved = offset;
		Rift::Reset(offset);
		g_visitedMaps.clear();
	}
}

void Timer::Pause() {
	if (g_running && !g_paused) {
		g_saved += GetServerTick() - g_base;
		g_paused = true;
		// Make sure rift is up-to-date
		Rift::Update(Timer::GetTicks());
	}
}

void Timer::Resume() {
	if (g_running && g_paused) {
		std::string map = GetMap();

		bool visited = false;
		for (std::string &v : g_visitedMaps) {
			if (map == v) {
				visited = true;
				break;
			}
		}

		if (!visited) {
			Rift::Split(Timer::GetTicks());
			g_visitedMaps.push_back(map);
		}

		g_base = GetServerTick();
		g_paused = false;
	}
}

void Timer::AddPauseTick() {
	if (g_running) {
		++g_saved;
	}
}

uint32_t Timer::GetTicks() {
	return g_saved + (g_running && !g_paused ? GetServerTick() - g_base : 0);
}
