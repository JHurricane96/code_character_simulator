#ifndef DRIVERS_MAIN_DRIVER
#define DRIVERS_MAIN_DRIVER

#include <chrono>
#include "main_driver.h"

namespace drivers {

state::State * MakeState() {
	state::State * state = new state::State(
		state::Terrain(std::vector<std::vector<state::TerrainElement> >()),
		std::vector<std::vector<std::shared_ptr<state::Actor> > >(2, std::vector<std::shared_ptr<state::Actor> >()),
		std::vector<std::shared_ptr<state::King> >(2, std::shared_ptr<state::King>()),
		std::vector<std::shared_ptr<state::Base> >(2, std::shared_ptr<state::Base>()),
		std::vector<std::shared_ptr<state::Flag> >(2, std::shared_ptr<state::Flag>()),
		std::vector<std::vector<std::shared_ptr<state::Tower> > >(2, std::vector<std::shared_ptr<state::Tower> >()),
		std::vector<std::vector<std::shared_ptr<state::Archer> > >(2, std::vector<std::shared_ptr<state::Archer> >())
	);
	return state;
}

MainDriver::MainDriver(
	player::PlayerAi p1_code,
	player::PlayerAi p2_code,
	std::shared_ptr<state::State> s1,
	std::shared_ptr<state::State> s2,
	std::shared_ptr<state::State> s3) :
	game_state(s1),
	p1_state_buffer(s2),
	p2_state_buffer(s3),
	p1_buffer(new state::PlayerStateHandler(p1_state_buffer.get(), PLAYER1)),
	p2_buffer(new state::PlayerStateHandler(p2_state_buffer.get(), PLAYER2)),
	p1_driver(new PlayerDriver(p1_buffer, p1_code)),
	p2_driver(new PlayerDriver(p2_buffer, p2_code)),
	game_over(false) {}

void MainDriver::GlobalUpdateLoop() {
	bool modified1, modified2;
	while(1) {
		if (game_over) {
			break;
		}
		modified1 = modified2 = false;
		if(p1_driver->GetIsModifyDone()) {
			game_state->MergeWithBuffer(*p1_state_buffer, PLAYER1);
			modified1 = true;
		}
		if (p2_driver->GetIsModifyDone()) {
			game_state->MergeWithBuffer(*p2_state_buffer, PLAYER2);
			modified2 = true;
		}
		game_state->Update(0.8);
		if (modified1) {
			p1_state_buffer->MergeWithMain(*game_state);
		}
		if (modified2) {
			p2_state_buffer->MergeWithMain(*game_state);
		}
		if (modified1) {
			p1_driver->SetIsModifyDone(false);
		}
		if (modified2) {
			p2_driver->SetIsModifyDone(false);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void MainDriver::Run() {
	p1_driver->Run();
	p2_driver->Run();
	runner = std::thread(&MainDriver::GlobalUpdateLoop, this);
}

void MainDriver::Stop() {
	StopP1();
	StopP2();
	game_over = true;
	runner.join();
}

void MainDriver::StopP1() {
	p1_driver->Stop();
}

void MainDriver::StopP2() {
	p2_driver->Stop();
}

float MainDriver::LogTimeRatio() {
	return p1_driver->Time() / p2_driver->Time();
}

}

#endif