/**
 * @file populate.cc
 * Function definitions for data transfer class functions
*/
#include <iostream>
#include <string>
#include "state.h"
#include "utilities.h"
#include "ipc.h"
#include "state.pb.h"

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
int PopulateActors(IPC::Actor* ActorMessage, std::shared_ptr<state::State>* StateVar) {

	PlayerId P1 = PLAYER1;
	PlayerId P2 = PLAYER2;

	std::vector<std::shared_ptr<Actor> > P1_Actors = GetPlayerActors(PlayerId P1);
	std::vector<std::shared_ptr<Actor> > P2_Actors = GetPlayerActors(PlayerId P2);

	for (auto actor1 : P1_Actors)
	{
		ActorMessage->set_id(actor1->GetId());
		ActorMessage->set_player_id(actor1->GetPlayerId())
	
		auto pos = actor1->GetPosition();

		ActorMessage->set_pos_x(pos.x)
		ActorMessage->set_pos_y(pos.y)

		if(actor1->GetAttackTarget() == NULL)
			ActorMessage->set_attack(false);
		else
			ActorMessage->set_attack(true);

		ActorMessage->set_hp(actor1->GetHp());
		ActorMessage->set_max_hp(actor1->GetMaxHp());

	}

	for (auto actor2 : P2_Actors)
	{
		ActorMessage->set_id(actor2->GetId());
		ActorMessage->set_player_id(actor2->GetPlayerId())
	
		auto pos = actor2->GetPosition();

		ActorMessage->set_pos_x(pos.x)
		ActorMessage->set_pos_y(pos.y)

		if(actor2->GetAttackTarget() == NULL)
			ActorMessage->set_attack(false);
		else
			ActorMessage->set_attack(true);

		ActorMessage->set_hp(actor2->GetHp());
		ActorMessage->set_max_hp(actor2->GetMaxHp());

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

	int DataTransfer::StateTransfer(std::shared_ptr<state::State> StateVar) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::State StateMessage;

		if (PopulateActors(&StateVar, StateMessage->set_allocated_terrain()) < 0) {
			std::cerr << "Failed to load terrain" << endl;
			return -1;
		}

		if (!StateMessage.SerializeToOstream(std::cout)) {
			std::cerr << "Failed to transfer state message" << endl;
			return -1;
		}
		return 0;
	}
}
