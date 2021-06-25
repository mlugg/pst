#include "Timer.hpp"
#include "Main.hpp"
#include "Utils.hpp"

static uint32_t g_saved;
static uint32_t g_base;
static bool g_running;
static bool g_paused;

void Timer::Start(uint32_t offset) {
	g_saved = offset;
	g_base = GetServerTick();
	g_running = true;
	g_paused = false;
}

void Timer::Stop() {
	if (g_running) {
		g_saved += GetServerTick() - g_base;
		g_running = false;
		g_paused = false;
	}
}

void Timer::Pause() {
	if (g_running && !g_paused) {
		g_saved += GetServerTick() - g_base;
		g_paused = true;
	}
}

void Timer::Resume() {
	if (g_running && g_paused) {
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
	return g_saved + (g_running ? GetServerTick() - g_base : 0);
}
