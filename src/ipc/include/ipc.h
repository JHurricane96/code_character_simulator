/**
 * @file ipc.h
 * Function declarations for inter process communication methods between
 * renderer and simulator
*/
#ifndef IPC_IPC_H
#define IPC_IPC_H

#include "state.h"

namespace ipc {

/**
 * Inter process communication methods between renderer and simulator
 * Utilises Protocol Buffers for serializing structured data
 */
	int StateTransfer (std::shared_ptr<state::State>& StateVar);

}
#endif // IPC_IPC_H
