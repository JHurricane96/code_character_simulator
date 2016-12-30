/**
 * @file populate.cc
 * Function definitions for data transfer class functions
*/
#include <iostream>
#include <string>
#include <fstream>
#include "state.h"
#include "utilities.h"
#include "actor/actor.h"
#include "ipc.h"
#include "state.pb.h"

using namespace std;
using namespace state;
using namespace physics;


int DepopulateActors(const IPC::State& RetrievedMessage) {

	for (int i = 0; i < RetrievedMessage.actors_size(); i++)
	{
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
	 * Populates the state message
	 *
	 * State message consists of actors & terrain
	 *
	 * @param[in]  StateVar  the state object
	 *
	 * @return     Exit status
	 */

	int depop() {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::State RetrievedMessage;

		fstream input("file.txt", ios::in | ios::binary);

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

int main()
{
	ipc::depop();
	return 0;
}