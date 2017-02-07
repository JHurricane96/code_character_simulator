/**
 * @file main_driver.h
 * Headers for the MainDriver class
 */

#ifndef DRIVERS_MAIN_DRIVER_H
#define DRIVERS_MAIN_DRIVER_H

#include <memory>
#include <atomic>

#include "state.h"
#include <vector>
#include "player_state_handler/player_state_handler.h"
#include "player_driver.h"
#include "player_ai.h"
#include "utilities.h"
#include "drivers_export.h"

namespace drivers {

/**
 * Runs the Main game
 * 
 * Controls 2 PlayerDriver classes running concurrently
 */
class DRIVERS_EXPORT MainDriver {
private:
	/**
	 * Main game State object
	 */
	std::shared_ptr<state::State> game_state;
	/**
	 * Player 1's copy of main game State object
	 */
	std::shared_ptr<state::State> p1_state_buffer;
	/**
	 * Player 2's copy of main game State object
	 */
	std::shared_ptr<state::State> p2_state_buffer;
	/**
	 * Player 1's Buffer Handler for State object
	 */
	std::shared_ptr<state::PlayerStateHandler> p1_buffer;
	/**
	 * Player 2's Buffer Handler for state object
	 */
	std::shared_ptr<state::PlayerStateHandler> p2_buffer;
	/**
	 * Player 1's PlayerDriver object
	 */
	std::shared_ptr<PlayerDriver> p1_driver;
	/**
	 * Player 2's PlayerDriver object
	 */
	std::shared_ptr<PlayerDriver> p2_driver;
	/**
	 * Thread object in which GlobalUpdateLoop function of MainDriver class runs
	 */
	std::thread runner;
	/**
	 * Boolean variable set True if game over else False
	 */
	std::atomic<bool> game_over;
	/**
	 * The total duration of the simulation in milliseconds
	 */
	int64_t total_game_duration;
	/**
	 * FPS at which the simulation will try to run at
	 *
	 * FPS will not exceed this value, but it may be below it
	 */
	int64_t fps;
	/**
	 * true if the simulation is running headless,
	 * false if running with a renderer
	 */
	bool is_headless;
	/**
	 * Infinite loop handling all the game Updates
	 */
	void GlobalUpdateLoop();
	/**
	 * Infinite loop handling all the game Updates
	 *
	 * This loop is for running the simulator without a renderer
	 */
	void GlobalUpdateLoopHeadless();
	/**
	 * Logs a Time Ratio between the 2 PlayerDriver objects
	 *
	 * @return     Ratio of Total Time consumed by the 2 PlayerDriver objects
	 */
	float LogTimeRatio();
public:
	MainDriver(
		player::PlayerAi p1_code,
		player::PlayerAi p2_code,
		std::shared_ptr<state::State> s1,
		std::shared_ptr<state::State> s2,
		std::shared_ptr<state::State> s3,
		int64_t total_game_duration,
		bool is_headless
	);
	/**
	 * Creates a thread whose Handler Function is the GlobalUpdateLoop
	 */
	void Run();
	/**
	 * Calls join on the runner thread
	 */
	void Join();
	/**
	 * Stops the GlobalUpdateLoop thread
	 * 
	 * Must be called only after Ending both PlayerDriver threads
	 */
	void Stop();
	/**
	 * Stops the PlayerDriver thread of Player 1
	 */
	void StopP1();
	/**
	 * Stops the PlayerDriver thread of Player 2
	 */
	void StopP2();
};

}

#endif
