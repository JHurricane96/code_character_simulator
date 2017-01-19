/**
 * @file ipc.h
 * Function declarations for inter process communication methods between
 * renderer and simulator
*/
#ifndef IPC_IPC_H
#define IPC_IPC_H

#include "state.h"

namespace ipc {

class Interrupts {

private:
		/**
		 * Describe simulation status
		 * True: Play, False: Pause
		 */
		bool play;

		/**
		 * Describe level number
		 */
		int level;

		/**
		 * Describe the exit status
		 */
		bool exit;

		/**
		 * Describe the restart status
		 */
		bool restart;

public:
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

};

/**
 * Inter process communication methods between renderer and simulator
 * Utilises Protocol Buffers for serializing structured data
 */
	int StateTransfer (std::shared_ptr<state::State> StateVar, double i);
	int StoreTerrain(state::Terrain TerrainVar);
	state::Terrain LoadTerrain();
	int IncomingInterrupts();
	int DepopulateStuff();
}
#endif // IPC_IPC_H
