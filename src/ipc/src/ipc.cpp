#include <ipc.h>
#include <mutex>

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

std::vector<std::string> Logger::GetLogs() {
	std::lock_guard<std::mutex> lock(logmutex);
	return logs;
}

void Logger::SetLogs(std::string log) {
	std::lock_guard<std::mutex> lock(logmutex);
	logs.push_back(log);
}

void Logger::EmptyLogs() {
	std::lock_guard<std::mutex> lock(logmutex);
	logs.clear();
}

Logger& Logger::Instance() {

	static Logger static_instance;
	return static_instance;
}

}
