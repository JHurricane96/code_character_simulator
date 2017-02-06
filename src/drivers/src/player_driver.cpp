#ifndef DRIVERS_PLAYER_DRIVER
#define DRIVERS_PLAYER_DRIVER

#include "player_driver.h"

namespace drivers {

PlayerDriver::PlayerDriver(std::shared_ptr<state::PlayerStateHandler> player_buffer, player::PlayerAi player_code) :
	is_modify_done(false),
	game_over(false),
	is_paused(false),
	code(player_code)
{
	total_time = 0;
	buffer = player_buffer;
}

bool PlayerDriver::GetIsModifyDone() {
	return std::atomic_load(&is_modify_done);

}

void PlayerDriver::SetIsModifyDone(bool val) {
	std::atomic_store(&is_modify_done, val);
}

void PlayerDriver::UpdateLoop() {
	while(1) {
		if (game_over) {
			break;
		}
		if(is_modify_done) continue;
		clock_t clocker = clock();
		code.Update(buffer);
		total_time += clock() - clocker;
		is_modify_done = true;
	}
}

void PlayerDriver::Run() {
	runner = std::thread(&PlayerDriver::UpdateLoop, this);
}

void PlayerDriver::Stop() {
	game_over = true;
	runner.join();
}

float PlayerDriver::Time() {
	return total_time;
}

}

#endif
