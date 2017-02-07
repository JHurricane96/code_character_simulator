/**
 * @file guard_rules.cpp
 * Implementation file of the Guard Strategies and Utilities for a Single Actor Unit per tick
 */

#include "guard_rules.h"

using namespace std;

namespace ai {

GuardRules::GuardRules(std::unique_ptr<std::vector<state::act_id_t>> kingsGuardParam, std::unique_ptr<std::vector<state::act_id_t>> flagsGuardParam, state::FormationMaker *formationParam = new DefaultFormation()) {
	formation = formationParam;
	kingsGuard = kingsGuardParam;
	flagsGuard = flagsGuardParam;
}
void GuardRules::Strategy(
	state::act_id_t unitId, 
	std::shared_ptr<state::PlayerStateHandler> state, 
	std::vector<state::act_id_t> &kingsGuard, 
	std::vector<state::act_id_t> &flagsGuard, 
	int &groupUtilityHolder,
	std::vector<std::pair<int64_t, int>> sortedEnemies
	) {
	if (
		state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::MAGICIAN &&
		state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::SWORDSMAN
	) return;
	auto terrainElementSize = state->OffsetToTerrainElement(physics::Vector2D(0, 0), nullptr).size;
	auto lim = terrainElementSize * state->GetTerrainRows() - 10;
	int isKingsGuard = -1, isFlagsGuard = -1;
	for (int i = 0; i < *kingsGuard.size(); i++) {
		if (*kingsGuard[i] == unitId) isKingsGuard = i;
	}
	for (int i = 0; i < *flagsGuard.size(); i++) {
		if (*flagsGuard[i] == unitId) isFlagsGuard = i;
	}
	if (isFlagsGuard == -1 && flagsGuard.size() < 4) {
		flagsGuard.push_back(unitId);
		isFlagsGuard = flagsGuard.size() - 1;
	}
	else if (isKingsGuard == -1 && kingsGuard.size() < 4) {
		kingsGuard.push_back(unitId);
		isKingsGuard = kingsGuard.size() - 1;
	}
	if (isKingsGuard >= 0) {
		state::act_id_t optimalEnemy = GetOptimalTarget(state, state->GetKing().GetId(), sortedEnemies, terrainElementSize * 2);
		if (optimalEnemy != -1 && state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			kingsGuard.erase(kingsGuard.begin() + isKingsGuard);
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
			return;
		}
		if (isKingsGuard == 0) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.x -= terrainElementSize * 2;
			if (dest.x < 0) dest.x = 10;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isKingsGuard == 1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.x += terrainElementSize * 2;
			if (dest.x > lim) dest.x = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isKingsGuard == 2) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.y -= terrainElementSize * 2;
			if (dest.y < 0) dest.y = 10;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isKingsGuard == 3) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.y += terrainElementSize * 2;
			if (dest.y > lim) dest.y = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		return;
	}
	else if (isFlagsGuard >= 0) {
		state::act_id_t optimalEnemy = GetOptimalTarget(state, state->GetFlag().GetId(), sortedEnemies, terrainElementSize * 2);
		if (optimalEnemy != -1 && state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			flagsGuard.erase(flagsGuard.begin() + isFlagsGuard);
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
			return;
		}
		if (isFlagsGuard == 0) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.x -= terrainElementSize * 2;
			if (dest.x < 0) dest.x = 10;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isFlagsGuard == 1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.x += terrainElementSize * 2;
			if (dest.x > lim) dest.x = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isFlagsGuard == 2) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.y -= terrainElementSize * 2;
			if (dest.y < 0) dest.y = 10;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		else if (isFlagsGuard == 3) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.y += terrainElementSize * 2;
			if (dest.y > lim) dest.y = lim;
			state->MoveUnits(movers, std::vector<physics::Vector2D>({dest}), formation, nullptr);
		}
		return;
	}
	if (state->GetFlag().GetVelocity().magnitude() > 0 || GetEnemyAllyHpRatio(state, state->GetFlag().GetId(), 10) > 1) {
		state::act_id_t optimalEnemy = GetOptimalTarget(state, state->GetFlag().GetId(), sortedEnemies, terrainElementSize * 2);
		if (optimalEnemy != -1 && state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			if (optimalEnemy == -1)
				state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetFlag().GetPosition()}), formation, nullptr);
			else
				state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(optimalEnemy, nullptr).GetPosition()}), formation, nullptr);
		}
	}
	else if (GetEnemyAllyHpRatio(state, state->GetKing().GetId(), 10) > 1) {
		state::act_id_t optimalEnemy = GetOptimalTarget(state, state->GetKing().GetId(), sortedEnemies, terrainElementSize * 2);
		if (optimalEnemy != -1 && state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			if (optimalEnemy == -1)
				state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetKing().GetPosition()}), formation, nullptr);
			else
				state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(optimalEnemy, nullptr).GetPosition()}), formation, nullptr);
		}
	}
	else if (GetEnemyAllyHpRatio(state, unitId, 10) > 1) {
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
	else if (IsEnemyTowerDominating(state)) {	
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
			state::act_id_t optimalEnemy = GetOptimalTarget(state, weakestTowerId, sortedEnemies, terrainElementSize * 2);
			if (optimalEnemy != -1 && state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
				state::list_act_id_t attackers;
				attackers.push_back(unitId);
				state->AttackUnit(attackers, optimalEnemy, nullptr);
			}
			else {
				state::list_act_id_t movers;
				movers.push_back(unitId);
				if (optimalEnemy == -1)
					state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(weakestTowerId, nullptr).GetPosition()}), formation, nullptr);
				else 
					state->MoveUnits(movers, std::vector<physics::Vector2D>({state->GetUnitFromId(optimalEnemy, nullptr).GetPosition()}), formation, nullptr);
			}
		}
		else {
			groupUtilityHolder = 1;
		}
	} 
	else {
		groupUtilityHolder = 1;
	}
}

int GuardRules::Utility(int &groupUtilityHolder) {
	return groupUtilityHolder;
}

}

