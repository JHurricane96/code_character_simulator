/**
 * @file ipc.h
 * Function declarations for inter process communication methods between renderer and simulator
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
        * Sends a boolean to the simulator to trigger a start simulation/stop simulation event
        * @param[in]  InterruptSignal    true for start, false for pause
        */
        void InterruptCall (bool InterruptSignal);
        /**
         * Sends an std::State variable to the renderer
         * @param[in]  StateVar    Internal state of the game
         */
        void StateTransfer (std::shared_ptr<state::State> StateVar);
        /**
         * Sends an std::string to the renderer which contains debug output present on simulation process's stdout
         * @param[in]  DebugOut    Process's stdout stream data piped to the renderer
         */
        void Debug(std::string DebugOut);

    };

}
#endif // IPC_IPC_H
