/**
 * @file player_driver.h
 * Headers for the PlayerDriver class
 */

#ifndef DRIVERS_PLAYER_DRIVER_H
#define DRIVERS_PLAYER_DRIVER_H

#include <memory>
#include <atomic>
#include <thread>
#include <ctime>

#include "state.h"
#include "player_state_handler/player_state_handler.h"
#include "player_ai.h"

namespace drivers {

/**
 * Runs the updates of individual players and sends update requests to MainDriver
 */
class PlayerDriver {
private:
	/**
	 * Variable which indicates status of the Update Request sent to the MainDriver
	 * 
	 * The variable if true indicates that an update is already in queue and needs 
	 * to wait until cleared by the MainDriver's GlobalUpdateLoop
	 * 
	 * The variable if false indicates that update can be done and request to 
	 * MainDriver can be sent
	 */
	std::atomic<bool> is_modify_done;
	/**
	 * Boolean variable set True if game over else False
	 */
	std::atomic<bool> game_over;
	/**
	 * Time consumed by the PlayerDriver
	 */
	float total_time;
	/**
	 * Thread object in which UpdateLoop of PlayerDriver class runs
	 */
	std::thread runner;
	/**
	 * Player's Buffer Handler for State object
	 */
	std::shared_ptr<state::PlayerStateHandler> buffer;
	/**
	 * Player AI code
	 */
	player::PlayerAi code;
	/**
	 * Runs PlayerDriver Updates in an Infinite Loop
	 */
	void UpdateLoop();
public:
	PlayerDriver(std::shared_ptr<state::PlayerStateHandler> player_buffer, player::PlayerAi player_code);
	/**
	 * Gets the is_modify_done Boolean variable
	 *
	 * @return     The is_modify_done Boolean variable
	 */
	bool GetIsModifyDone();
	/**
	 * Sets the is_modify_done Boolean variable
	 *
	 * @param[in]  val   The Value of is_modify_done Boolean variable to be set
	 */
	void SetIsModifyDone(bool val);
	/**
	 * Creates a Thread whose Handler Function is the UpdateLoop
	 */
	void Run();
	/*
	 * Stops the UpdateLoop thread
	 */
	void Stop();
	/**
	 * Gives the Total Time consumed by the PlayerDriver
	 *
	 * @return     Time consumed
	 */
	float Time();
};

}

#endif
