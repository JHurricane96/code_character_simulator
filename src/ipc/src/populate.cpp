/**
 * @file populate.cc
 * Function definitions for data transfer class functions
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "state.h"
#include "utilities.h"
#include "actor/actor.h"
#include "ipc.h"
#include "state.pb.h"

using namespace std;
using namespace state;
using namespace physics;

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
int PopulateActors(shared_ptr<state::State> StateVar, IPC::State* StateMessage) {

	PlayerId P1 = PLAYER1;
	PlayerId P2 = PLAYER2;

	std::vector<std::shared_ptr<Actor> > P1_Actors = StateVar->GetPlayerActors(P1);
	std::vector<std::shared_ptr<Actor> > P2_Actors = StateVar->GetPlayerActors(P2);

	for (auto actor1 : P1_Actors)
	{

		IPC::State::Actor* ActorMessageP1 = StateMessage->add_actors();

		ActorMessageP1->set_id(actor1->GetId());
		ActorMessageP1->set_player_id(actor1->GetPlayerId());
	
		auto pos = actor1->GetPosition();

		ActorMessageP1->set_pos_x(pos.x);
		ActorMessageP1->set_pos_y(pos.y);

		if(actor1->GetAttackTarget() == NULL)
			ActorMessageP1->set_attack(false);
		else
			ActorMessageP1->set_attack(true);

		ActorMessageP1->set_hp(actor1->GetHp());
		ActorMessageP1->set_max_hp(actor1->GetMaxHp());

	}

	for (auto actor2 : P2_Actors)
	{
		IPC::State::Actor* ActorMessageP2 = StateMessage->add_actors();

		ActorMessageP2->set_id(actor2->GetId());
		ActorMessageP2->set_player_id(actor2->GetPlayerId());
	
		auto pos = actor2->GetPosition();

		ActorMessageP2->set_pos_x(pos.x);
		ActorMessageP2->set_pos_y(pos.y);

		if(actor2->GetAttackTarget() == NULL)
			ActorMessageP2->set_attack(false);
		else
			ActorMessageP2->set_attack(true);

		ActorMessageP2->set_hp(actor2->GetHp());
		ActorMessageP2->set_max_hp(actor2->GetMaxHp());

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

	int StateTransfer(std::shared_ptr<state::State> StateVar) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::State StateMessage;

		fstream output("file.txt", ios::out | ios::trunc | ios::binary);


		if (PopulateActors(StateVar, &StateMessage) < 0) {
			cerr << "Failed to load actors" << endl;
			return -1;
		}

		if (!StateMessage.SerializeToOstream(&output)) {
			cerr << "Failed to transfer state message" << endl;
			return -1;
		}
		/*while(true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			cout<<endl;
		}*/

		return 0;
	}
}
