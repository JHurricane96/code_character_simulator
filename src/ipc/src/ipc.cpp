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

void Interrupts::SetPlayStatus(bool play_status) {
	play = play_status;
}

void Interrupts::SetLevelNumber(int current_level) {
	level = current_level;
}

void Interrupts::SetExitStatus(bool exit_status) {
	exit = exit_status;
}

void Interrupts::SetRestartStatus(bool restart_status) {
	restart = restart_status;
}

}
