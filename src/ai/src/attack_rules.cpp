#include "player_state_handler/player_state_handler.h"
#include "attack_rules.h"
#include "ai_utilities.h"
#include "ipc.h"

#include <bits/stdc++.h>

#define INF 10000
#define TINY 0.00001
#define SWORDHIT 100
#define MAGEHIT 150
namespace ai {

AttackRules::AttackRules() {
	formation = new DefaultFormation();
}

/**
 * Strategy to effectively attack enemies whilst in attack state
 *
 * @param[in]  unitId  The unit identifier
 * @param[in]  state   The state
 */
void AttackRules::Strategy (
	state::act_id_t unitId,
	std::shared_ptr<state::PlayerStateHandler> state,
	std::vector<std::pair<int64_t, int>> sortedEnemies
)	{
		if (state->GetUnitFromId(unitId, nullptr).GetPathPlannerHelper().IsPathPlanning()
			|| ( !state->GetUnitFromId(unitId, nullptr).CanAttack()
			&& state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::KING )
			|| !state->GetUnitFromId(unitId, nullptr).CanPathPlan()
			|| IsAttackingEnemy(state, unitId)
		) {
			if(state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::KING)
				// ipc::Logger::Instance().SetLogs("Returning King");
				std::cerr << "Returning King\n";
			return;
		}

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

		if(unitView.GetActorType() == state::ActorType::KING) {
			// std::unique_ptr<int> flagCaptureStatus(new int());
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
			std::unique_ptr<int> success(new int());
			auto enemy_king = state -> GetEnemyKing(success.get());
			float AttackValidity = GetEnemyAllyHpRatio(state, unitId, 20);

			state::ActorType typeTower = state::ActorType::TOWER;
			auto nearestEnemyTower = NearestEnemy(state, unitId, &typeTower);

			/* Look for enemy's nearest tower */

			if( nearestEnemyTower.first != -1) {
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

			/* Try to attack king if in LOS */

			if (*success == 1) {
				if (InAttackRange(state, unitId, enemy_king)) {
					state::list_act_id_t attackers;
					attackers.push_back(unitId);
					state -> AttackUnit(attackers,enemy_king.GetId(), nullptr);
				}
				else {
					state::list_act_id_t attackers;
					attackers.push_back(unitId);
					std::vector<physics::Vector2D> tempPath;
					state->MoveUnits (
						attackers,
						std::vector<physics::Vector2D>({enemy_king.GetPosition()}),
						formation,
						nullptr);
				}
			}

			/* Check if wise to attack */

			else if (AttackValidity < 1 && AttackValidity != -1 || true) {
				auto unit = state -> GetUnitFromId (unitId, nullptr);
				auto weakestEnemy = WeakestEnemy(state, unitId, unit.GetAttackRange());

				/* Try to attack weakest enemy in LOS */

				if(weakestEnemy.second < unit.GetHp() && (weakestEnemy.first != -1)) {
					if( InAttackRange(state, unitId, state -> GetEnemyUnitFromId(weakestEnemy.first, nullptr)) ) {
						state::list_act_id_t attackers;
						attackers.push_back(unitId);
						state -> AttackUnit(attackers, weakestEnemy.first, nullptr);
					}
				}

				/* Otherwise pursue nearest enemy */

				else {

					state::ActorType typeScout = state::ActorType::SCOUT;
					state::ActorType typeBase = state::ActorType::BASE;
					state::ActorType typeSwordsman = state::ActorType::SWORDSMAN;
					state::ActorType typeMagician = state::ActorType::MAGICIAN;

					auto nearestEnemyScout = NearestEnemy(state, unitId, &typeScout);
					auto nearestEnemyBase = NearestEnemy(state, unitId, &typeBase);

					/* Look for enemy's nearest scout */

					if( nearestEnemyScout.first != -1
						&& InAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemyScout.first, nullptr)) ) {
							state::list_act_id_t attackers;
							attackers.push_back(unitId);
							state->AttackUnit(attackers, nearestEnemyScout.first, nullptr);
						}

					/* Look for enemy's base */

					else if( nearestEnemyBase.first != -1) {
						if (InAttackRange(state, unitId, state -> GetEnemyUnitFromId(nearestEnemyBase.first, nullptr)) ) {
							state::list_act_id_t attackers;
							attackers.push_back(unitId);
							state->AttackUnit(attackers, nearestEnemyBase.first, nullptr);
						}
						else {
							auto target = state->GetEnemyUnitFromId (nearestEnemyBase.first, nullptr);
							state::list_act_id_t attackers;
							attackers.push_back(unitId);
							std::vector<physics::Vector2D> tempPath;
							state->MoveUnits(attackers, std::vector<physics::Vector2D>({target.GetPosition()}), formation, nullptr);
						}
					}

					/* Look for nearest enemy swordsman/mage */

					else {
						auto nearestEnemySwordsman = NearestEnemy(state, unitId, &typeSwordsman);
						auto nearestEnemyMagician = NearestEnemy(state, unitId, &typeMagician);
						std::pair<state::act_id_t, float> target(-1, FLT_MAX);

						if(nearestEnemySwordsman.first == -1 && nearestEnemyMagician.first != -1) {
							target = nearestEnemyMagician;
						}
						else if(nearestEnemySwordsman.first != -1 && nearestEnemyMagician.first == -1) {
							target = nearestEnemySwordsman;
						}
						else if(nearestEnemySwordsman.first != -1 && nearestEnemyMagician.first != -1) {
							auto swordsmanUnit = state -> GetEnemyUnitFromId(nearestEnemySwordsman.first, nullptr);
							auto magicianUnit = state -> GetEnemyUnitFromId(nearestEnemyMagician.first, nullptr);

							target = ( (MAGEHIT/(magicianUnit.GetHp() + TINY)) > (SWORDHIT/(swordsmanUnit.GetHp() + TINY)) )
							? nearestEnemyMagician : nearestEnemySwordsman;
						}

						if(target.first != -1) {
							if (InAttackRange(state, unitId, state -> GetEnemyUnitFromId(target.first, nullptr)) ) {
								state::list_act_id_t attackers;
								attackers.push_back(unitId);
								state->AttackUnit(attackers, target.first, nullptr);
							}
							else {
								auto targetUnit = state->GetEnemyUnitFromId (target.first, nullptr);
								state::list_act_id_t attackers;
								attackers.push_back(unitId);
								std::vector<physics::Vector2D> tempPath;
								state->MoveUnits(attackers, std::vector<physics::Vector2D>({targetUnit.GetPosition()}), formation, nullptr);
							}
						}
					}
				}
			}
		}
}

int AttackRules::Utility(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state) {

	if(state->GetEnemyUnitFromId (unitId, nullptr).GetActorType() == state::ActorType::KING)
		return 1;

	float enemyAllyHpRatio = GetEnemyAllyHpRatio(state, unitId, 20);
	float distFromEnemyBase = GetDistanceFromUnit(state, unitId, state -> GetEnemyBase().GetPosition());
	float distFromBase = GetDistanceFromUnit(state, unitId, state -> GetBase().GetPosition());
	float distFromKing = GetDistanceFromUnit(state, unitId, state -> GetKing().GetPosition());

	std::unique_ptr<int> success(new int());
	auto enemy_king = state -> GetEnemyKing(success.get());
	float distFromEnemyKing = INF;
	if (*success == 1) {
		distFromEnemyKing = GetDistanceFromUnit(state, unitId, enemy_king.GetPosition());
	}

	bool isAttackingKing = IsAttackingKing(state, unitId);
	bool isAttackingEnemy = IsAttackingEnemy(state, unitId);

	float eaHPratioKing = GetEnemyAllyHpRatio(state, state -> GetKing().GetId(), 20);
	float eaHPratioBase = GetEnemyAllyHpRatio(state, state -> GetBase().GetId(), 20);

	if (isAttackingKing) return 1;

	float attackUtility = (1.0 / enemyAllyHpRatio) * (1.0 / sqrt(distFromBase) + 1.0 / distFromEnemyBase + 1.0 / sqrt(distFromKing) + 1.0 / sqrt(distFromEnemyKing)); 
	float guard_utility = relu(eaHPratioBase - 0.8) + relu(eaHPratioKing - 1);
	float retreat_utility = 1. / (attackUtility + TINY);
	float explore_utility = 1. / distFromBase + 1. / (attackUtility + guard_utility + TINY);

	std::vector <float> utilities(4);
	utilities.push_back(attackUtility);
	utilities.push_back(guard_utility);
	utilities.push_back(retreat_utility);
	utilities.push_back(explore_utility);

	float maxutility = *std::max_element(utilities.begin(), utilities.end());
	if (maxutility == attackUtility) return 1;
	if (maxutility == guard_utility) return 4;
	if (maxutility == retreat_utility) return 2;
	if (maxutility == explore_utility) return 3;
}

}
