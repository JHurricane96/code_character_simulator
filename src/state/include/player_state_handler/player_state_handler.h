#ifndef STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H
#define STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H

#include <memory>
#include <cstdint>
#include "vector2d.h"
#include "actor/actor.h"
#include "state.h"
#include "terrain/terrain_element.h"
#include "state_export.h"
#include "utilities.h"

namespace state {

struct TerrainElementView {
	physics::Vector2D position;
	int64_t size;
	TERRAIN_TYPE terrain_type;
	LOS_TYPE los_type;
};

class PathPlannerHelperView {
private:
	std::shared_ptr<PathPlannerHelper> path_planner_helper;
public:
	bool IsPathPlanning();
};

class UnitView {
private:
	std::shared_ptr<Actor> unit;
public:
	physics::Vector2D GetPosition();
	act_id_t GetId();
	int64_t GetHp();
	int64_t GetSpeed();
	act_id_t GetAttackTargetId();
	physics::Vector2D GetVelocity();
	physics::Vector2D GetPosition();
	PathPlannerHelperView GetPathPlannerHelper();
	void SetAttackTargetId(act_id_t attack_target_id);
};

class STATE_EXPORT StateView {
private:
	std::shared_ptr<State> state;
public:
	StateView(State &state);
	list_act_id_t GetUnitIds();
	list_act_id_t GetEnemyIds();
	list_act_id_t GetTowerIds();
	list_act_id_t GetFlagIds();
	TerrainElementView XYToTerrainElement();
	TerrainElementView OffsetToTerrainElement();
	UnitView GetUnitFromId();
};

class STATE_EXPORT PlayerStateHandler {
private:
	const StateView state_view;
	act_id_t player_id;
public:
	PlayerStateHandler(State &state);
	void MoveUnits(
		list_act_id_t unit_ids,
		physics::Vector2D destination,
		const std::unique_ptr<Formation> formation_type
	);
	void AttackUnit(list_act_id_t attacker_ids, act_id_t attack_target_id);
	int64_t PlanPath();
	StateView GetState();
};

}

#endif
