/**
 * @file interrupts.cpp
 * Function definitions for listening to interrupts from renderer
*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include "ipc.h"

/**
 * Extracts the interrupts from the string pattern
 *
 * - WXXYZ is the pattern of the interrupt
 * - Each digit can be 1 or 2
 *
 * - W	: 	PlayStatus
 * - XX	:	LevelNumber
 * - Y	: 	ExitStatus
 * - Z	:	RestartStatus
 *
 * @param[in]  Interrupt     The interrupt
 * @param      InterruptVar  The interrupt variable
 */

void DepopulateInterrupt(int Interrupt, ipc::Interrupts* InterruptVar) {

	int Status = 0, Count = 0;

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
	 * @param[in]	InterruptVar	the interrupts object
	 */

	void IncomingInterrupts(ipc::Interrupts* InterruptVar) {

		bool flag = true;

		while(flag) {

			std::string InterruptMessage;

			std::cin >> InterruptMessage;

			int Interrupt = atoi (InterruptMessage.c_str());

			if(Interrupt == 0)
				flag = false;
			else
				DepopulateInterrupt(Interrupt, InterruptVar);
		}
		return;
	}
}
