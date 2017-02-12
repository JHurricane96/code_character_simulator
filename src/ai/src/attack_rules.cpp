#include "player_state_handler/player_state_handler.h"
#include "attack_rules.h"
#include "ai_utilities.h"
#include "ipc.h"

#include <bits/stdc++.h>

namespace ai {

AttackRules::AttackRules() {
	formation = new DefaultFormation();
}

void AttackRules::Strategy (
	state::act_id_t unitId,
	std::shared_ptr<state::PlayerStateHandler> state,
	int &groupUtilityHolder,
	std::vector<std::pair<int64_t, int>> sortedEnemies,
	int aiLevel
)	{
		if (( !state->GetUnitFromId(unitId, nullptr).CanAttack()
			&& state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::KING )
			|| ( !state->GetUnitFromId(unitId, nullptr).CanPathPlan() 
			&& state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::TOWER )
		) {
			groupUtilityHolder = 4;
			return;
		}

		groupUtilityHolder = 1;

		std::vector<int64_t> terrain_weights(3);
		if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::MAGICIAN) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}
		else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SWORDSMAN) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}
		else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::KING) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}
		else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SCOUT) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}

		auto unitView = state->GetUnitFromId(unitId, nullptr);
		state::list_act_id_t traveller;
		std::vector<physics::Vector2D> tempPath;

		if((state->GetFlag().GetVelocity().magnitude() != 0 )
		) {
			groupUtilityHolder = 4;
			return;
		}

		if (unitView.GetActorType() == state::ActorType::TOWER) {

			state::ActorType typeTower = state::ActorType::KING;

			auto kingTarget = NearestEnemy(state, unitId, &typeTower);
			if(kingTarget.first != -1) {
				if ( InAttackRange(state, unitId, state->GetEnemyUnitFromId(kingTarget.first, nullptr))) {
					traveller.push_back(unitId);
					state->AttackUnit(traveller, kingTarget.first, nullptr);
				}
			}
			else {
				auto target = NearestEnemy(state, unitId);
				if(target.first != -1) {
					if ( InAttackRange(state, unitId, state->GetEnemyUnitFromId(kingTarget.first, nullptr))) {
						traveller.push_back(unitId);
						state->AttackUnit(traveller, target.first, nullptr);
					}
				}
			}
			return;
		}

		else if(unitView.GetActorType() == state::ActorType::KING) {
			
			if(unitView.GetPathPlannerHelper().IsPathPlanning())
				return;

			int flagCaptureStatus;
			std::unique_ptr<int> flagDropStatus(new int());

			state->FlagCapture(&flagCaptureStatus);
			switch(flagCaptureStatus) {
				case -1:
					traveller.push_back(unitId);
					state->MoveUnits (
						traveller,
						std::vector<physics::Vector2D>({state->GetEnemyBase().GetPosition()}),
						formation,
						nullptr);
					break;
				case -2:
					state->FlagDrop(flagDropStatus.get());
					switch(*flagDropStatus) {
						case -1:
							traveller.push_back(unitId);
							state->MoveUnits (
								traveller,
								std::vector<physics::Vector2D>({state->GetBase().GetPosition()}),
								formation,
								nullptr);
							break;
					}
				case 1:
					traveller.push_back(unitId);
					state->MoveUnits (
						traveller,
						std::vector<physics::Vector2D>({state->GetBase().GetPosition()}),
						formation,
						nullptr);
					break;
			}
		}

		else {

			/* Look for enemy's nearest tower */

			state::ActorType typeTower = state::ActorType::TOWER;
			auto nearestEnemyTower = NearestEnemy(state, unitId, &typeTower);

			if( nearestEnemyTower.first != -1 && aiLevel == 3) {
				if (InAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemyTower.first, nullptr)) ) {
					state::list_act_id_t attackers;
					attackers.push_back(unitId);
					state->AttackUnit(attackers, nearestEnemyTower.first, nullptr);
				}
				else {
					auto target = state->GetEnemyUnitFromId (nearestEnemyTower.first, nullptr);
					state::list_act_id_t attackers;
					attackers.push_back(unitId);
					std::vector<physics::Vector2D> tempPath;
					state->MoveUnits(attackers, std::vector<physics::Vector2D>({target.GetPosition()}), formation, nullptr);
				}
			}

			else {

				auto OptimalTarget = GetOptimalTarget(state,unitId,sortedEnemies, 20);
				if(OptimalTarget != -1) {
					if ( InAttackRange(state, unitId, state->GetEnemyUnitFromId(OptimalTarget, nullptr)) ) {
						traveller.push_back(unitId);
						state->AttackUnit(traveller, OptimalTarget, nullptr);
					}
				}
				else {
					auto target = NearestEnemy(state, unitId);
					if(target.first != -1) {
						traveller.push_back(unitId);
						state->MoveUnits (
							traveller, 
							std::vector<physics::Vector2D>({state->GetEnemyUnitFromId(target.first, nullptr).GetPosition()}), 
							formation, 
							nullptr);
					}
				}

			}

		}

}


int AttackRules::Utility(int &groupUtilityHolder) {

	return 	groupUtilityHolder;
}

}
