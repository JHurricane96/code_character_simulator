#ifndef DRIVERS_MAIN_DRIVER
#define DRIVERS_MAIN_DRIVER

#include <chrono>
#include "main_driver.h"
#include "ipc.h"

namespace drivers {

MainDriver::MainDriver(
	player::PlayerAi p1_code,
	player::PlayerAi p2_code,
	std::shared_ptr<state::State> s1,
	std::shared_ptr<state::State> s2,
	std::shared_ptr<state::State> s3,
	int64_t total_game_duration,
	bool is_headless) :
	game_state(s1),
	p1_state_buffer(s2),
	p2_state_buffer(s3),
	p1_buffer(new state::PlayerStateHandler(p1_state_buffer.get(), state::PLAYER1)),
	p2_buffer(new state::PlayerStateHandler(p2_state_buffer.get(), state::PLAYER2)),
	p1_driver(new PlayerDriver(p1_buffer, p1_code)),
	p2_driver(new PlayerDriver(p2_buffer, p2_code)),
	game_over(false),
	total_game_duration(total_game_duration),
	fps(30),
	is_headless(is_headless) {}

void MainDriver::GlobalUpdateLoop() {
	bool modified1, modified2;

	ipc::Interrupts* InterruptVar(new ipc::Interrupts);
	std::thread RendererInput(ipc::IncomingInterrupts, InterruptVar);

	std::chrono::milliseconds game_duration(0);

	auto prev_time = std::chrono::high_resolution_clock::now();

	while(1) {
		if (game_over) {
			break;
		}

		auto start_time = std::chrono::high_resolution_clock::now();
		auto update_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			start_time - prev_time
		);
		game_duration += update_duration;
		prev_time = start_time;

		modified1 = modified2 = false;
		if(p1_driver->GetIsModifyDone()) {
			game_state->MergeWithBuffer(*p1_state_buffer, state::PLAYER1);
			modified1 = true;
		}
		if (p2_driver->GetIsModifyDone()) {
			game_state->MergeWithBuffer(*p2_state_buffer, state::PLAYER2);
			modified2 = true;
		}
		game_state->Update((float) update_duration.count() / fps);
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

		if (game_duration.count() >= total_game_duration) {
			ipc::StateTransfer(game_state, true);
			break;
		}
		else {
			ipc::StateTransfer(game_state, false);
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto calculcation_duration =
			std::chrono::duration_cast<std::chrono::milliseconds>(
				end_time - start_time
			);
		std::this_thread::sleep_for(std::chrono::milliseconds(
			std::max<long>((long)0, 1000 / fps - calculcation_duration.count()))
		);

		if (!InterruptVar->GetPlayStatus()) {
			auto pause_start_time = std::chrono::high_resolution_clock::now();
			p1_driver->Pause();
			p2_driver->Pause();
			while (!InterruptVar->GetPlayStatus());
			p1_driver->Resume();
			p2_driver->Resume();
			auto pause_end_time = std::chrono::high_resolution_clock::now();
			auto pause_duration =
				std::chrono::duration_cast<std::chrono::milliseconds>(
					pause_end_time - pause_start_time
				);
			prev_time += pause_duration;
		}
	}
	RendererInput.join();
	Stop();
}

void MainDriver::GlobalUpdateLoopHeadless() {
	bool modified1, modified2;

	std::chrono::milliseconds game_duration(0);

	auto prev_time = std::chrono::high_resolution_clock::now();

	while(1) {
		if (game_over) {
			break;
		}

		auto start_time = std::chrono::high_resolution_clock::now();
		auto update_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
			start_time - prev_time
		);
		game_duration += update_duration;
		prev_time = start_time;

		modified1 = modified2 = false;
		if(p1_driver->GetIsModifyDone()) {
			game_state->MergeWithBuffer(*p1_state_buffer, state::PLAYER1);
			modified1 = true;
		}
		if (p2_driver->GetIsModifyDone()) {
			game_state->MergeWithBuffer(*p2_state_buffer, state::PLAYER2);
			modified2 = true;
		}
		game_state->Update((float) update_duration.count() / fps);
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

		if (game_duration.count() >= total_game_duration) {
			break;
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto calculcation_duration =
			std::chrono::duration_cast<std::chrono::milliseconds>(
				end_time - start_time
			);
		std::this_thread::sleep_for(std::chrono::milliseconds(
			std::max<long>((long)0, 1000 / fps - calculcation_duration.count()))
		);
	}
	Stop();
}

void MainDriver::Run() {
	game_state->Update(1);
	p1_state_buffer->MergeWithMain(*game_state);
	p2_state_buffer->MergeWithMain(*game_state);

	p1_driver->Run();
	p2_driver->Run();
	if (!is_headless) {
		runner = std::thread(&MainDriver::GlobalUpdateLoop, this);
	}
	else {
		runner = std::thread(&MainDriver::GlobalUpdateLoopHeadless, this);
	}
}

void MainDriver::Join() {
	runner.join();
}

void MainDriver::Stop() {
	StopP1();
	StopP2();
	game_over = true;
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
