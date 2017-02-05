/**
 * @file ipc.h
 * Function declarations for inter process communication methods between
 * renderer and simulator
*/
#ifndef IPC_IPC_H
#define IPC_IPC_H

#include "state.h"
#include "ipc_export.h"
#include <atomic>
#include <mutex>

namespace ipc {

/**
 * Interrupts from the renderer to handle user interaction
 */
class IPC_EXPORT Interrupts {

private:
		/**
		 * Describe simulation status
		 * - True: Play
		 * - False: Pause
		 */
		std::atomic<bool> play;

		/**
		 * Describe level number
		 */
		std::atomic<int> level;

		/**
		 * Describe the exit status
		 */
		std::atomic<bool> exit;

		/**
		 * Describe the restart status
		 */
		std::atomic<bool> restart;

public:
		/**
		 * Constructor for Interrupts
		 */
		Interrupts();

		/**
		 * Gets the state of the simulation
		 *
		 * @return     play status
		 */
		bool GetPlayStatus();

		/**
		 * Gets the level number of currently loaded level
		 *
		 * @return     The level number
		 */
		int GetLevelNumber();

		/**
		 * Gets the exit status of the simulator
		 *
		 * @return     The exit status
		 */
		bool GetExitStatus();

		/**
		 * Gets the restart status of the simulator
		 *
		 * @return     The restart status
		 */
		bool GetRestartStatus();

		/**
		 * Sets the play status
		 *
		 * @param[in]  play_status  The play status
		 */
		void SetPlayStatus(bool play_status);

		/**
		 * Sets the level number
		 *
		 * @param[in]  current_level  The current level
		 */
		void SetLevelNumber(int current_level);

		/**
		 * Sets the exit status
		 *
		 * @param[in]  exit_status  The exit status
		 */
		void SetExitStatus(bool exit_status);

		/**
		 * Sets the restart status
		 *
		 * @param[in]  restart_status  The restart status
		 */
		void SetRestartStatus(bool restart_status);

};

class IPC_EXPORT Logger {

	std::vector<std::string> logs;
	std::mutex logmutex;

	public:

		/**
		 * Gets the logs
		 *
		 * @return     The logs
		 */
		std::vector<std::string> GetLogs();

		/**
		 * Sets the logs
		 *
		 * @param[in]  log   The log
		 */
		void SetLogs(std::string log);

		/**
		 * Empties the logger
		 */
		void EmptyLogs();

		/**
		 * Creates a singleton of the logger
		 *
		 * @return     returns a logger instance
		 */
		static Logger& Instance();
};

/**
 * Inter process communication methods between renderer and simulator
 * Utilises Protocol Buffers for serializing structured data
 */
	/**
	 * Passes the state to renderer every update
	 *
	 * @param[in]  StateVar    The state variable
	 * @param[in]  ExitStatus  The exit status
	 */
	IPC_EXPORT void StateTransfer (std::shared_ptr<state::State> StateVar, bool ExitStatus);

	/**
	 * Stores a terrain
	 *
	 * @param[in]  TerrainVar  The terrain variable
	 * @param[in]  filename    The filename
	 */
	IPC_EXPORT void StoreTerrain(state::Terrain TerrainVar, std::string filename);

	/**
	 * Loads a terrain
	 *
	 * @param[in]  filename  The filename
	 *
	 * @return     Returns a state::Terrain
	 */
	IPC_EXPORT state::Terrain LoadTerrain(std::string filename);

	/**
	 * Listen to the renderer for an interrupt
	 *
	 * Interrupts can be of 4 types:
	 * - PlayStatus
	 * - LevelNumber
	 * - ExitStatus
	 * - RestartStatus
	 *
	 * @param      InterruptVar  The interrupt variable
	 */
	IPC_EXPORT void IncomingInterrupts(Interrupts* InterruptVar);
}
#endif // IPC_IPC_H
