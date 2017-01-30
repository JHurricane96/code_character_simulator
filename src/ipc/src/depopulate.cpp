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
#include "terrain.pb.h"

using namespace std;


int DepopulateActors(const IPC::State& RetrievedMessage) {

	for (int i = 0; i < RetrievedMessage.actors_size(); i++)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(1));

		cout<<"In here"<<endl;
		const IPC::State::Actor& ActorMessage = RetrievedMessage.actors(i);

		cout<<"Actor ID: "<<ActorMessage.id()<<endl;
		cout<<"Actor PlayerID: "<<ActorMessage.player_id()<<endl;
		cout<<"Actor PosX: "<<ActorMessage.x()<<endl;
		cout<<"Actor PosY: "<<ActorMessage.y()<<endl;
		
		if(ActorMessage.is_attacking() == false)
			cout<<"Actor attack target: False"<<endl;
		else
			cout<<"Actor attack target: True"<<endl;
		
		cout<<"Actor HP: "<<ActorMessage.hp()<<endl;
		cout<<"Actor Max HP: "<<ActorMessage.max_hp()<<endl;

		switch(ActorMessage.actor_type()){

			case IPC::State::Actor::ARCHER	:
				cout<<"Actor Type: ARCHER\n";
				break;
			case IPC::State::Actor::ARROW	:
				cout<<"Actor Type: ARROW\n";
				break;
			case IPC::State::Actor::BASE		:
				cout<<"Actor Type: BASE\n";
				break;
			case IPC::State::Actor::FLAG		:
				cout<<"Actor Type: FLAG\n";
				break;
			case IPC::State::Actor::KING 		:
				cout<<"Actor Type: KING\n";
				break;
			case IPC::State::Actor::SCOUT		:
				cout<<"Actor Type: SCOUT\n";
				break;
			case IPC::State::Actor::SWORDSMAN	:
				cout<<"Actor Type: SWORDSMAN\n";
				break;
			case IPC::State::Actor::TOWER		:
				cout<<"Actor Type: TOWER\n";
				break;

		}
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

	int DepopulateStuff() {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::State RetrievedMessage;

		fstream input("file.txt", ios::in | ios::binary);
		istream *in = &cin;

		if (!RetrievedMessage.ParseFromIstream(&input)) {
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
