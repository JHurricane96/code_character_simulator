/**
 * @file depopulate.cc
 * Function definitions for data transfer class functions
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "ipc.h"
#include "state.pb.h"

using namespace std;


int DepopulateActors(const IPC::State& RetrievedMessage) {

	for (int i = 0; i < RetrievedMessage.actors_size(); i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		cout<<"In here"<<endl;
		const IPC::State::Actor& ActorMessage = RetrievedMessage.actors(i);

		cout<<"Actor ID: "<<ActorMessage.id()<<endl;
		cout<<"Actor PosX: "<<ActorMessage.pos_x()<<endl;
		cout<<"Actor PosY: "<<ActorMessage.pos_y()<<endl;
		
		if(ActorMessage.attack() == false)
			cout<<"Actor attack target: False"<<endl;
		else
			cout<<"Actor attack target: True"<<endl;
		
		cout<<"Actor HP: "<<ActorMessage.hp()<<endl;
		cout<<"Actor Max HP: "<<ActorMessage.max_hp()<<endl;
	}

	return 0;
}

namespace ipc {

	/**
	 * Depopulates the incoming interrupts
	 *
	 * Interrupts could be game level, play/pause state or console logs
	 *
	 * @return     Exit status
	 */

	int IncomingInterrupts() {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::State RetrievedMessage;

		fstream input("file2.txt", ios::in | ios::binary);
		istream *in = &cin;

		if (!RetrievedMessage.ParseFromIstream(in)) {
			cerr << "Failed to retrieve state message" << endl;
			return -1;
		}

		if (DepopulateActors(RetrievedMessage) < 0) {
			cerr << "Failed to load actors from file" << endl;
			return -1;
		}

		return 0;
	}
}
