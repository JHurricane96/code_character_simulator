/**
 * @file interrupts.cpp
 * Function definitions for listening to interrupts from renderer
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "state.h"
#include "utilities.h"
#include "actor/actor.h"
#include "ipc.h"
#include "interrupts.pb.h"

using namespace std;
using namespace state;
using namespace physics;

int DepopulateInterrupt(IPC::Interrupts& InterruptMessage) {

	switch(InterruptMessage.interrupt_case()) {

		case IPC::Interrupts::kPlayPauseStatus	:
			return 1;
		case IPC::Interrupts::kLevelNumber		:
			return InterruptMessage.level_number();
		case IPC::Interrupts::kExitStatus		:
			return 3;
		case IPC::Interrupts::kRestartStatus	:
			return 4;
		case IPC::Interrupts::INTERRUPT_NOT_SET	:
			return 0;
	}
}

namespace ipc {

	/**
	 * Listens to interrupts from the renderer
	 *
	 * @return     Exit status
	 */

	int IncomingInterrupts() {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Interrupts InterruptMessage;

		if (!InterruptMessage.ParseFromIstream(&std::cin)) {
			cerr << "Failed to retrieve interrupt" << endl;
			return -1;
		}

		if (DepopulateInterrupt(InterruptMessage) < 0) {
			cerr << "Failed to depopulate interrupt" << endl;
			return -1;
		}
	}
}
