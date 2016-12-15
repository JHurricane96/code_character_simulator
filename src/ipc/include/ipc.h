/**
 * @file ipc.h
 * Function declarations for inter process communication methods between
 * renderer and simulator
*/
#ifndef IPC_IPC_H
#define IPC_IPC_H

#include <memory>
#include <string>
#include "state.h"

namespace ipc {

/**
 * Inter process communication methods between renderer and simulator
 * Utilises Protocol Buffers for serializing structured data
 */

class DataTransfer {

public:
		/**
		* Sends a boolean to the simulator to trigger a start simulation/stop
		* simulation event
		*
		* @param[in]  PlayPauseStatus  true for start, false for pause
		*
		* @return     Exit status
		*/
		int InterruptCall (bool PlayPauseStatus);
		/**
		* Sends level number to the simulator
		*
		* @param[in]  LevelNumber  current level number
		*
		* @return     Exit status
		*/
		int UpdateLevel (int LevelNumber);
		/**
		 * Sends an std::string to the renderer which contains debug output
		 * present on simulation process's stdout
		 *
		 * @param[in]  DebugOut    Process's stdout piped to the renderer
		 *
		 * @return     Exit status
		 */
		int DebugLogs (std::string DebugOut);
		/**
		 * Sends an std::State variable to the renderer
		 *
		 * @param[in]  StateVar    Internal state of the game
		 *
		 * @return     Exit status
		 */
		int StateTransfer (std::shared_ptr<state::State> StateVar);
	};

}
#endif // IPC_IPC_H
