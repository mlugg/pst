#include "Rift.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SPLIT_FIFO_NAME "/tmp/rift_portal_split"
#define TO_MICROSECS(tick) (tick * 15000)

static int g_fd = -1;

void Rift::Init() {
	if (mkfifo(SPLIT_FIFO_NAME, 0644)) {
		if (errno == EEXIST) {
			unlink(SPLIT_FIFO_NAME);
			mkfifo(SPLIT_FIFO_NAME, 0644);
		}
	}
}

void Rift::Attach() {
	if (g_fd != -1) return;
	g_fd = open(SPLIT_FIFO_NAME, O_WRONLY | O_NONBLOCK);
}

void Rift::Cleanup() {
	if (g_fd) {
		close(g_fd);
		unlink(SPLIT_FIFO_NAME);
	}
}

void Rift::Start(uint32_t tick) {
	if (!g_fd) return;
	if (dprintf(g_fd, "%d BEGIN\n", TO_MICROSECS(tick)) < 0) {
		close(g_fd);
		g_fd = -1;
	}
}

void Rift::Split(uint32_t tick) {
	if (!g_fd) return;
	if (dprintf(g_fd, "%d SPLIT\n", TO_MICROSECS(tick)) < 0) {
		close(g_fd);
		g_fd = -1;
	}
}

void Rift::Reset(uint32_t tick) {
	if (!g_fd) return;
	if (dprintf(g_fd, "%d RESET\n", TO_MICROSECS(tick)) < 0) {
		close(g_fd);
		g_fd = -1;
	}
}

void Rift::Update(uint32_t tick) {
	if (!g_fd) return;
	if (dprintf(g_fd, "%d\n", TO_MICROSECS(tick)) < 0) {
		close(g_fd);
		g_fd = -1;
	}
}
