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

void GuardRules::Strategy(state::act_id_t unitId, std::shared_ptr<state::PlayerStateHandler> state, std::vector<state::act_id_t> &kingsGuard, std::vector<state::act_id_t> &flagsGuard, int &groupUtilityHolder) {
	auto map_bounds = state->OffsetToTerrainElement(physics::Vector2D(0, 0)).size;
	auto lim = map_bounds * state->GetTerrainRows() - 1;
	if (flagsGuard.size() < 4) {
		flagsGuard.push_back(unitId);
	}
	else if (kingsGuard.size() < 4) {
		kingsGuard.push_back(unitId);
	}
	int isKingsGuard = -1, isFlagsGuard = -1;
	for (int i = 0; i < kingsGuard.size(); i++) {
		if (kingsGuard[i] == unitId) isKingsGuard = i;
	}
	for (int i = 0; i < flagsGuard.size(); i++) {
		if (flagsGuard[i] == unitId) isFlagsGuard = i;
	}
	if (isKingsGuard >= 0) {
		state::act_id_t optimalEnemy = GetOptimalEnemy(state, state->GetKing().GetId(), 5);
		if (state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			kingsGuard.erase(kingsGuard.begin() + isKingsGuard);
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
			return;
		}
		if (i == 0) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.x -= 5;
			if (dest.x < 0) dest.x = 0;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		else if (i == 1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.x += 5;
			if (dest.x > lim) dest.x = lim;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		else if (i == 2) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.y -= 5;
			if (dest.y < 0) dest.y = 0;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		else if (i == 3) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetKing().GetPosition();
			dest.y += 5;
			if (dest.y > lim) dest.y = lim;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		return;
	}
	else if (isFlagsGuard >= 0) {
		state::act_id_t optimalEnemy = GetOptimalEnemy(state, state->GetFlag().GetId(), 5);
		if (state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			flagsGuard.erase(flagsGuard.begin() + isFlagsGuard);
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
			return;
		}
		if (i == 0) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.x -= 5;
			if (dest.x < 0) dest.x = 0;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		else if (i == 1) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.x += 5;
			if (dest.x > lim) dest.x = lim;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		else if (i == 2) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.y -= 5;
			if (dest.y < 0) dest.y = 0;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		else if (i == 3) {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			physics::Vector2D dest = state->GetFlag().GetPosition();
			dest.y += 5;
			if (dest.y > lim) dest.y = lim;
			state->MoveUnits(movers, dest, formation, nullptr);
		}
		return;
	}
	if (state->GetFlag().GetVelocity().magnitude() > 0 || GetEnemyAllyHpRatio(state, state->GetFlag().GetId(), 5) > 1) {
		state::act_id_t optimalEnemy = GetOptimalEnemy(state, state->GetFlag().GetId(), 5);
		if (state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, state->GetUnitFromId(optimalEnemy).GetPosition(), formation, nullptr);
		}
	}
	else if (GetEnemyAllyHpRatio(state, state->GetKing().GetId(), 5) > 1) {
		state::act_id_t optimalEnemy = GetOptimalEnemy(state, state->GetKing().GetId(), 5);
		if (state->GetUnitFromId(optimalEnemy, nullptr).GetPosition().distance(state->GetUnitFromId(unitId, nullptr).GetPosition()) <= state->GetUnitFromId(unitId, nullptr).GetAttackRange()) {
			state::list_act_id_t attackers;
			attackers.push_back(unitId);
			state->AttackUnit(attackers, optimalEnemy, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, state->GetUnitFromId(optimalEnemy).GetPosition(), formation, nullptr);
		}
	}
	else if (GetEnemyAllyHpRatio(state, unitId, 5) > 1) {
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
			state->MoveUnits(movers, state->GetUnitFromId(weakestTowerId).GetPosition(), formation, nullptr);
		}
		else {
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, state->GetBase().GetPosition(), formation, nullptr);
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
			state::list_act_id_t movers;
			movers.push_back(unitId);
			state->MoveUnits(movers, state->GetUnitFromId(weakestTowerId).GetPosition(), formation, nullptr);
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

