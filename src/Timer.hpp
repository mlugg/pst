#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdint.h>

#define IPT (200.0f / 3.0f)

namespace Timer {
	void Start(uint32_t offset = 0);
	void Stop();
	void Reset(uint32_t offset = 0);

	void Pause();
	void Resume();

	void AddPauseTick();

	uint32_t GetTicks();
};

#endif
