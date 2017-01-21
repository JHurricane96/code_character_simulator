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

void DepopulateInterrupt(IPC::Interrupts& InterruptMessage, ipc::Interrupts* InterruptVar) {

	switch(InterruptMessage.interrupt_case()) {

		case IPC::Interrupts::kPlayStatus		:
			InterruptVar->SetPlayStatus(InterruptMessage.play_status());
			break;
		case IPC::Interrupts::kLevelNumber		:
			InterruptVar->SetLevelNumber(InterruptMessage.level_number());
			break;
		case IPC::Interrupts::kExitStatus		:
			InterruptVar->SetExitStatus(InterruptMessage.exit_status());
			break;
		case IPC::Interrupts::kRestartStatus	:
			InterruptVar->SetRestartStatus(InterruptMessage.restart_status());
			break;
		case IPC::Interrupts::INTERRUPT_NOT_SET	:
			break;
	}
	return;
}

namespace ipc {

	/**
	 * Listens to interrupts from the renderer
	 *
	 * @return     Exit status
	 */

	void IncomingInterrupts(ipc::Interrupts* InterruptVar) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Interrupts InterruptMessage;

		if (!InterruptMessage.ParseFromIstream(&std::cin)) {
			cerr << "Failed to retrieve interrupt" << endl;
		}

		DepopulateInterrupt(InterruptMessage, InterruptVar);
	}
}
