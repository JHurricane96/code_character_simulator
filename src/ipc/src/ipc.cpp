#include <ipc.h>

namespace ipc {

Interrupts::Interrupts() :
	play(false),
	level(1),
	exit(false),
	restart(false) {}

bool Interrupts::GetPlayStatus() {
	return play.load();
}

int Interrupts::GetLevelNumber() {
	return level.load();
}

bool Interrupts::GetExitStatus() {
	return exit.load();
}

bool Interrupts::GetRestartStatus() {
	return restart.load();
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
