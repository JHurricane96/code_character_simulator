/**
 * @file populate.cpp
 * Function definitions for state transfer
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

	int ActorLength = P1_Actors.size() + P2_Actors.size();

	bool ScoutVisibleP1 = !StateVar->GetEnemyScouts(P2).empty();
	bool ScoutVisibleP2 = !StateVar->GetEnemyScouts(P1).empty();

	bool TowerFlag = false;

	for (auto actor1 : P1_Actors)
	{

		IPC::State::Actor* ActorMessageP1 = StateMessage->add_actors();

		ActorMessageP1->set_id(actor1->GetId());
		ActorMessageP1->set_player_id(actor1->GetPlayerId());
	
		physics::Vector2D pos = actor1->GetPosition();

		ActorMessageP1->set_x(pos.x);
		ActorMessageP1->set_y(pos.y);

		if(actor1->GetAttackTarget() == nullptr)
			ActorMessageP1->set_is_attacking(false);
		else {
			ActorMessageP1->set_is_attacking(true);

			IPC::State::Vector2D AttackTargetPosition;
			physics::Vector2D attack_pos = actor1->GetAttackTarget()->GetPosition();

			AttackTargetPosition.set_x(attack_pos.x);
			AttackTargetPosition.set_y(attack_pos.y);

			ActorMessageP1->set_allocated_attack_target_position(&AttackTargetPosition);
		}

		ActorMessageP1->set_hp(actor1->GetHp());
		ActorMessageP1->set_max_hp(actor1->GetMaxHp());
		ActorMessageP1->set_is_moving(actor1->GetVelocity().magnitude() != 0);

		IPC::State::Vector2D* Dest = new IPC::State::Vector2D;

		if (actor1->CanPathPlan() && actor1->GetPathPlannerHelper()->IsPathPlanning()) {
			auto pph = actor1->GetPathPlannerHelper();
			auto Destination = pph->GetDestination();

			Dest->set_x(Destination.x);
			Dest->set_y(Destination.y);

			ActorMessageP1->set_allocated_destination(Dest);
		}

		state::ActorType typevar = actor1->GetActorType();

		switch(typevar){

			case state::ActorType::MAGICIAN	:
				ActorMessageP1->set_actor_type(IPC::State::Actor::MAGICIAN);
				break;
			case state::ActorType::FIREBALL	:
				ActorMessageP1->set_actor_type(IPC::State::Actor::FIREBALL);
				break;
			case state::ActorType::BASE		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::BASE);
				break;
			case state::ActorType::FLAG		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::FLAG);
				ActorMessageP1->set_is_being_carried(static_pointer_cast<Flag>(actor1)->IsCaptured());
				break;
			case state::ActorType::KING 		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::KING);
				ActorMessageP1->set_is_carrying_flag(static_pointer_cast<King>(actor1)->HasFlag());
				break;
			case state::ActorType::SCOUT		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::SCOUT);
				ActorMessageP1->set_is_visible_to_enemy(ScoutVisibleP1);
				break;
			case state::ActorType::SWORDSMAN	:
				ActorMessageP1->set_actor_type(IPC::State::Actor::SWORDSMAN);
				break;
			case state::ActorType::TOWER		:
				ActorMessageP1->set_actor_type(IPC::State::Actor::TOWER);
				ActorMessageP1->set_contention_meter_score(static_pointer_cast<Tower>(actor1)->GetContentionScore());
				if(!TowerFlag) {
					StateMessage->set_contention_meter_limit(static_pointer_cast<Tower>(actor1)->GetMaxContentionScore());
					TowerFlag=true;
				}
				break;

		}

	}

	for (auto actor2 : P2_Actors)
	{
		IPC::State::Actor* ActorMessageP2 = StateMessage->add_actors();

		ActorMessageP2->set_id(actor2->GetId());
		ActorMessageP2->set_player_id(actor2->GetPlayerId());
	
		physics::Vector2D pos = actor2->GetPosition();

		ActorMessageP2->set_x(pos.x);
		ActorMessageP2->set_y(pos.y);

		if(actor2->GetAttackTarget() == nullptr)
			ActorMessageP2->set_is_attacking(false);
		else {
			ActorMessageP2->set_is_attacking(true);

			IPC::State::Vector2D AttackTargetPosition;
			physics::Vector2D attack_pos = actor2->GetAttackTarget()->GetPosition();

			AttackTargetPosition.set_x(attack_pos.x);
			AttackTargetPosition.set_y(attack_pos.y);

			ActorMessageP2->set_allocated_attack_target_position(&AttackTargetPosition);
		}

		ActorMessageP2->set_hp(actor2->GetHp());
		ActorMessageP2->set_max_hp(actor2->GetMaxHp());
		ActorMessageP2->set_is_moving(actor2->GetVelocity().magnitude() != 0);

		IPC::State::Vector2D* Dest = new IPC::State::Vector2D;

		if (actor2->CanPathPlan() && actor2->GetPathPlannerHelper()->IsPathPlanning()) {
			auto pph = actor2->GetPathPlannerHelper();
			auto Destination = pph->GetDestination();

			Dest->set_x(Destination.x);
			Dest->set_y(Destination.y);

			ActorMessageP2->set_allocated_destination(Dest);
		}

		state::ActorType typevar2 = actor2->GetActorType();

		switch(typevar2){

			case state::ActorType::MAGICIAN	:
				ActorMessageP2->set_actor_type(IPC::State::Actor::MAGICIAN);
				break;
			case state::ActorType::FIREBALL	:
				ActorMessageP2->set_actor_type(IPC::State::Actor::FIREBALL);
				break;
			case state::ActorType::BASE		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::BASE);
				break;
			case state::ActorType::FLAG		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::FLAG);
				ActorMessageP2->set_is_being_carried(static_pointer_cast<Flag>(actor2)->IsCaptured());
				break;
			case state::ActorType::KING 		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::KING);
				ActorMessageP2->set_is_carrying_flag(static_pointer_cast<King>(actor2)->HasFlag());
				break;
			case state::ActorType::SCOUT		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::SCOUT);
				ActorMessageP2->set_is_visible_to_enemy(ScoutVisibleP2);
				break;
			case state::ActorType::SWORDSMAN	:
				ActorMessageP2->set_actor_type(IPC::State::Actor::SWORDSMAN);
				break;
			case state::ActorType::TOWER		:
				ActorMessageP2->set_actor_type(IPC::State::Actor::TOWER);
				ActorMessageP2->set_contention_meter_score(static_pointer_cast<Tower>(actor2)->GetContentionScore());
				break;

		}

	}
	StateMessage->set_no_of_actors(ActorLength);

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

		fstream output("file.txt", ios::out | ios::trunc | ios::binary);


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

		std::cout << std::flush;
		return 0;
	}
}
