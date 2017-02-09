/**
 * @file guard_rules.cpp
 * Implementation file of the Guard Strategies and Utilities for a Single Actor Unit per tick
 */

#include "guard_rules.h"

using namespace std;

namespace ai {

GuardRules::GuardRules(state::FormationMaker *formationParam) {
	formation = formationParam;
}

void GuardRules::Strategy(
	state::act_id_t unitId, 
	std::shared_ptr<state::PlayerStateHandler> state, 
	std::vector<state::act_id_t> &kingsGuard, 
	std::vector<state::act_id_t> &flagsGuard, 
	int &groupUtilityHolder,
	std::vector<std::pair<int64_t, int>> sortedEnemies
	) {
	auto unitView = state->GetUnitFromId(unitId, nullptr);
	if(unitView.GetActorType() == state::ActorType::KING) {
		groupUtilityHolder = 1;
	}
	else
	{
		auto terrainElementSize = state->OffsetToTerrainElement(physics::Vector2D(0, 0), nullptr).size;
		auto lim = terrainElementSize * state->GetTerrainRows() - 5;
		float maxUnitRange = state->GetMagicians()[0].GetAttackRange();
		std::vector<state::ActorType> scSwMaTypes(3);
		scSwMaTypes[0] = state::ActorType::SCOUT;
		scSwMaTypes[1] = state::ActorType::SWORDSMAN;
		scSwMaTypes[2] = state::ActorType::MAGICIAN;
		if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SCOUT && 
			state->GetUnitFromId(unitId, nullptr).GetVelocity().magnitude() == 0) {
			auto myVec = state->GetUnitFromId(unitId, nullptr).GetPosition();
			auto baseVec = state->GetBase().GetPosition();
			auto enemyBaseVec = state->GetEnemyBase().GetPosition();
			if (myVec == enemyBaseVec) {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				state->MoveUnits(movers, std::vector<physics::Vector2D>({baseVec}), formation, nullptr);
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				state->MoveUnits(movers, std::vector<physics::Vector2D>({enemyBaseVec}), formation, nullptr);
			}
		}
		if (
			state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::MAGICIAN &&
			state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::SWORDSMAN
		) return;
		int isKingsGuard = -1, isFlagsGuard = -1;
		for (int i = 0; i < kingsGuard.size(); i++) {
			if (kingsGuard[i] == unitId){
				isKingsGuard = i;
				break;
			}
		}
		for (int i = 0; i < flagsGuard.size(); i++) {
			if (flagsGuard[i] == unitId){ 
				isFlagsGuard = i;
				break;
			}
		}
		if (isFlagsGuard == -1 && flagsGuard.size() < 4 && state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::MAGICIAN) {
			flagsGuard.push_back(unitId);
			isFlagsGuard = flagsGuard.size() - 1;
		}
		else if (isKingsGuard == -1 && kingsGuard.size() < 4 && state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::MAGICIAN) {
			kingsGuard.push_back(unitId);
			isKingsGuard = kingsGuard.size() - 1;
		}
		if (state->GetUnitFromId(unitId, nullptr).GetHp() == 0) {
			if (isKingsGuard >= 0)kingsGuard.erase(kingsGuard.begin() + isKingsGuard);
			else if (isFlagsGuard >= 0)flagsGuard.erase(flagsGuard.begin() + isFlagsGuard);
			return;
		}
		if (isKingsGuard >= 0) {
			state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetKing().GetId(), scSwMaTypes);
			if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state->AttackUnit(attackers, nearestEnemy, nullptr);
				return;
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				physics::Vector2D dest = state->GetKing().GetPosition();
				state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
			}
			return;
		}
		else if (isFlagsGuard >= 0) {
			state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetFlag().GetId(), scSwMaTypes);
			if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state->AttackUnit(attackers, nearestEnemy, nullptr);
				return;
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				physics::Vector2D dest = state->GetFlag().GetPosition();
				state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
			}
			return;
		}
		if (state->GetFlag().GetVelocity().magnitude() > 0 || GetEnemyAllyHpRatioByThreshold(state, state->GetFlag().GetId(), maxUnitRange) > 1) {
			state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetFlag().GetId(), scSwMaTypes);
			if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state->AttackUnit(attackers, nearestEnemy, nullptr);
				return;
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				physics::Vector2D dest = state->GetFlag().GetPosition();
				state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
			}
		}
		else if (GetEnemyAllyHpRatioByThreshold(state, state->GetKing().GetId(), maxUnitRange) > 1) {
			state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetKing().GetId(), scSwMaTypes);
			if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state->AttackUnit(attackers, nearestEnemy, nullptr);
				return;
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				physics::Vector2D dest = state->GetKing().GetPosition();
				state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
			}
		}
		else {
			state::act_id_t nearestEnemy = NearestEnemyByType(state, unitId, scSwMaTypes);
			if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state->AttackUnit(attackers, nearestEnemy, nullptr);
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				physics::Vector2D dest = state->GetUnitFromId(nearestEnemy, nullptr).GetPosition();
				state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
			}
			groupUtilityHolder = 1;
		}
	}
}

int GuardRules::Utility(int &groupUtilityHolder) {
	return groupUtilityHolder;
}

}