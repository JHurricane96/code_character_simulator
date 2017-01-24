/**
 * @file interrupts.cpp
 * Function definitions for listening to interrupts from renderer
*/
#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h>
#include "ipc.h"

using namespace std;

void DepopulateInterrupt(string InterruptMessage, ipc::Interrupts* InterruptVar) {

	int Interrupt = atoi (InterruptMessage);
	int Status = 0;
	int Count = 0;

	while(Interrupt > 0) {

		switch(Count) {
			case 0:
				Status = Interrupt % 10;
				if(Status == 1)
					InterruptVar->SetRestartStatus(false);
				else if(Status == 2)
					InterruptVar->SetRestartStatus(true);
				Interrupt/=10;
				break;
			case 1:
				Status = Interrupt % 10;
				if(Status == 1)
					InterruptVar->SetExitStatus(false);
				else if(Status == 2)
					InterruptVar->SetExitStatus(true);
				Interrupt/=10;
				break;
			case 2:
				Status = Interrupt % 100;
				InterruptVar->SetLevelNumber(Status);
				Interrupt/=100;
				break;
			case 3:
				Status = Interrupt % 10;
				if(Status == 1)
					InterruptVar->SetPlayStatus(false);
				else if(Status == 2)
					InterruptVar->SetPlayStatus(true);
				Interrupt/=10;
				break;
		}
		Count++;
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

		string InterruptMessage;

		std::cin >> InterruptMessage;

		DepopulateInterrupt(InterruptMessage, InterruptVar);
	}
}
