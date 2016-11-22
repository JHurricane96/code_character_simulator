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
	auto terrainElementSize = state->OffsetToTerrainElement(physics::Vector2D(0, 0), nullptr).size;
	auto lim = terrainElementSize * state->GetTerrainRows() - 5;
	float maxUnitRange = state->GetMagicians()[0].GetAttackRange();
	std::vector<state::ActorType> scSwMaTypes(3);
	std::vector<state::ActorType> scSwMaToTypes(4);
	scSwMaTypes[0] = scSwMaToTypes[0] = state::ActorType::SCOUT;
	scSwMaTypes[1] = scSwMaToTypes[1] = state::ActorType::SWORDSMAN;
	scSwMaTypes[2] = scSwMaToTypes[2] = state::ActorType::MAGICIAN;
	scSwMaToTypes[3] = state::ActorType::TOWER;
	if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::TOWER) {
		state::act_id_t nearestEnemy = NearestEnemyByType(state, unitId, scSwMaTypes);
		if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy, nullptr);
		}
	}
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
		else if (isKingsGuard == 0) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.x -= maxUnitRange;
			if (dest.x < 0) dest.x = 5;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isKingsGuard == 1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.x += maxUnitRange;
			if (dest.x > lim) dest.x = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isKingsGuard == 2) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.y -= maxUnitRange;
			if (dest.y < 0) dest.y = 5;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isKingsGuard == 3) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.y += maxUnitRange;
			if (dest.y > lim) dest.y = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		return;
	}
	else if (isFlagsGuard >= 0) {
		state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetFlag().GetId(), scSwMaTypes);
		if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
			flagsGuard.erase(flagsGuard.begin() + isFlagsGuard);
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy, nullptr);
			return;
		}
		else if (isFlagsGuard == 0) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.x -= maxUnitRange;
			if (dest.x < 0) dest.x = 5;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isFlagsGuard == 1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.x += maxUnitRange;
			if (dest.x > lim) dest.x = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isFlagsGuard == 2) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.y -= maxUnitRange;
			if (dest.y < 0) dest.y = 5;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isFlagsGuard == 3) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.y += maxUnitRange;
			if (dest.y > lim) dest.y = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		return;
	}
	
	if (state->GetUnitFromId(unitId, nullptr).GetVelocity().magnitude() != 0 && 
		state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SWORDSMAN)return;

	if (state->GetFlag().GetVelocity().magnitude() > 0 || GetEnemyAllyHpRatioByThreshold(state, state->GetFlag().GetId(), maxUnitRange) > 1) {
		state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetFlag().GetId(), scSwMaTypes);
		if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(nearestEnemy, nullptr).GetPosition()}), formation, nullptr);
		}
	}
	else if (GetEnemyAllyHpRatioByThreshold(state, state->GetKing().GetId(), maxUnitRange) > 1) {
		state::act_id_t nearestEnemy = NearestEnemyByType(state, state->GetKing().GetId(), scSwMaTypes);
		if (nearestEnemy != -1 && InAttackRange(state, unitId, state->GetEnemyUnitFromId(nearestEnemy, nullptr))) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, nearestEnemy, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(nearestEnemy, nullptr).GetPosition()}), formation, nullptr);
		}
	}
	else if (GetEnemyAllyHpRatioByThreshold(state, unitId, maxUnitRange) > 1) {
		state::act_id_t weakestTowerId = -1;
		float weakestTowerHp = FLT_MAX, hp;
		std::vector<state::TowerView> towers = state->GetTowers();
		for (int i = 0; i < towers.size(); i++) {
			hp = towers[i].GetHp();
			if (hp < weakestTowerHp) {
				weakestTowerHp = hp;
				weakestTowerId = towers[i].GetId();
			}
		}
		if (weakestTowerId != -1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(weakestTowerId, nullptr).GetPosition()}), formation, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetBase().GetPosition()}), formation, nullptr);
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
			state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(nearestEnemy, nullptr).GetPosition()}), formation, nullptr);
		}
		groupUtilityHolder = 1;
	}
}

int GuardRules::Utility(int &groupUtilityHolder) {
	return groupUtilityHolder;
}

}