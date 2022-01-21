#ifndef RIFT_HPP
#define RIFT_HPP

#include <stdint.h>

namespace Rift {
	void Init();
	void Attach();
	void Cleanup();
	void Start(uint32_t tick);
	void Split(uint32_t tick);
	void Reset(uint32_t tick);
	void Update(uint32_t tick);
};

#endif
