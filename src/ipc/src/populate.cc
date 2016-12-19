/**
 * @file populate.cc
 * Function definitions for data transfer class functions
*/
#include <iostream>
#include <string>
#include "state.h"
#include "ipc.h"
#include "state.pb.h"

/**
 * Populates the actors
 *
 * State actors are stored as repeated Actor messages as part of the State message
 *
 * @param[in]  StateVar        the state object
 * @param[in]  ActorMessage    the actor message object
 *
 * @return     Exit status
 */
int PopulateActors(IPC::Actor* ActorMessage, std::shared_ptr<state::State>* StateVar) {


	return 0;
}

namespace ipc {

	/**
	 * Populates the state message
	 *
	 * State message consists of actors & terrain
	 *
	 * @param[in]  StateVar  the state object
	 *
	 * @return     Exit status
	 */

	int DataTransfer::StateTransfer(std::shared_ptr<state::State> StateVar) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::State StateMessage;

		if (PopulateActors(&StateVar, StateMessage->set_allocated_terrain()) < 0) {
			std::cerr << "Failed to load terrain" << endl;
			return -1;
		}

		if (!StateMessage.SerializeToOstream(std::cout)) {
			std::cerr << "Failed to transfer state message" << endl;
			return -1;
		}
		return 0;
	}
}
