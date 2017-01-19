#include <ipc.h>

namespace ipc {

Interrupts::Interrupts(bool play, int level, bool exit, bool restart) :
	play(true),
	level(1),
	exit(false),
	restart(false) {}

bool Interrupts::GetPlayStatus() {
	return play;
}

int Interrupts::GetLevelNumber() {
	return level;
}

bool Interrupts::GetExitStatus() {
	return exit;
}

bool Interrupts::GetRestartStatus() {
	return restart;
}

}
