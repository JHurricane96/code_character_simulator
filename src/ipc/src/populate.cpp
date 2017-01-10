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
 * @param[in]  StateVar        the state::State object
 * @param[in]  StateMessage    the IPC::State message object
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

		state::ActorType typevar = actor1->GetActorType();

		switch(typevar){

			/*case state::MAGICIAN	:
				ActorMessageP1->set_actor_type(IPC::State::Actor::MAGICIAN);
				break;
			case state::FIREBALL	:
				ActorMessageP1->set_actor_type(IPC::State::Actor::FIREBALL);
				break;*/
			case state::ActorType::BASE		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::BASE);
				break;
			case state::ActorType::FLAG		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::FLAG);
				break;
			case state::ActorType::KING 		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::KING);
				break;
			case state::ActorType::SCOUT		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::SCOUT);
				break;
			case state::ActorType::SWORDSMAN	:
				ActorMessageP1->set_actor_type(IPC::State::Actor::SWORDSMAN);
				break;
			case state::ActorType::TOWER		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::TOWER);
				break;

		}

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

		state::ActorType typevar2 = actor2->GetActorType();

		switch(typevar2){

			/*case state::MAGICIAN	:
				ActorMessageP2->set_actor_type(IPC::State::Actor::MAGICIAN);
				break;
			case state::FIREBALL	:
				ActorMessageP2->set_actor_type(IPC::State::Actor::FIREBALL);
				break;*/
			case state::ActorType::BASE		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::BASE);
				break;
			case state::ActorType::FLAG		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::FLAG);
				break;
			case state::ActorType::KING 		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::KING);
				break;
			case state::ActorType::SCOUT		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::SCOUT);
				break;
			case state::ActorType::SWORDSMAN	:
				ActorMessageP2->set_actor_type(IPC::State::Actor::SWORDSMAN);
				break;
			case state::ActorType::TOWER		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::TOWER);
				break;

		}

	}

	return 0;
}

/**
 * Populates the LOS for player 1 and player 2
 *
 * State LOS for player 1 and 2 are stored as embedded LOS messages which
 * are 2D arrays of enum LOS_TYPE
 *
 * @param[in]  StateVar        the state::State object
 * @param[in]  StateMessage    the IPC::State message object
 *
 * @return     Exit status
 */
int PopulateLOS(shared_ptr<state::State> StateVar, IPC::State* StateMessage) {

	PlayerId P1 = PLAYER1;
	PlayerId P2 = PLAYER2;

	state::Terrain TerrainVar = StateVar->GetTerrain();

	/**
	 * Create constructed instances of LOS to pass to StateMessage
	 */
	IPC::State::LOS* LOSP1(new IPC::State::LOS);
	IPC::State::LOS* LOSP2(new IPC::State::LOS);

	/**
	 * Loop through the terrain to get LOS for each terrain element for each player
	 */
	int64_t size = TerrainVar.GetRows();

	for (double row = 0; row < size; ++row) {

		IPC::State::LOS::LOSRows* LOSRowsP1 = LOSP1->add_row();
		IPC::State::LOS::LOSRows* LOSRowsP2 = LOSP2->add_row();
		for (double col = 0; col < size; ++col) {

			physics::Vector2D offset;

			offset.x = row;
			offset.y = col;

			state::TerrainElement ElementVar = TerrainVar.OffsetToTerrainElement(offset);

			state::LOS_TYPE Player1LOS = ElementVar.GetLos(P1);
			state::LOS_TYPE Player2LOS = ElementVar.GetLos(P2);

			switch(Player1LOS){
				case state::UNEXPLORED :
					LOSRowsP1->add_element(IPC::State::LOS::UNEXPLORED);
					break;
				case state::EXPLORED :
					LOSRowsP1->add_element(IPC::State::LOS::EXPLORED);
					break;
				case state::DIRECT_LOS :
					LOSRowsP1->add_element(IPC::State::LOS::DIRECT_LOS);
					break;
			}
			switch(Player2LOS){
				case state::UNEXPLORED :
					LOSRowsP2->add_element(IPC::State::LOS::UNEXPLORED);
					break;
				case state::EXPLORED :
					LOSRowsP2->add_element(IPC::State::LOS::EXPLORED);
					break;
				case state::DIRECT_LOS :
					LOSRowsP2->add_element(IPC::State::LOS::DIRECT_LOS);
					break;
			}
		}
	}
	/**
	 * Pass instances of LOS to StateMessage for player 1 and 2
	 */
	StateMessage->set_allocated_player1_los(LOSP1);
	StateMessage->set_allocated_player2_los(LOSP2);

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

		fstream output("file2.txt", ios::out | ios::trunc | ios::binary);


		if (PopulateActors(StateVar, &StateMessage) < 0) {
			cerr << "Failed to load actors" << endl;
			return -1;
		}

		if (PopulateLOS(StateVar, &StateMessage) < 0) {
			cerr << "Failed to load actors" << endl;
			return -1;
		}

		if (!StateMessage.SerializeToOstream(&std::cout)) {
			cerr << "Failed to transfer state message" << endl;
			return -1;
		}

		return 0;
	}
}
