#ifndef STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H
#define STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H

#include <memory>
#include <cstdint>
#include "vector2d.h"
#include "terrain/terrain_element.h"
#include "state_export.h"

namespace state {

struct TerrainElementView {
	physics::Vector2D position;
	int64_t size;
	TERRAIN_TYPE terrain_type;
	LOS_TYPE los_type;
};

class UnitView {
private:

public:
	physics::Vector2D GetPosition();
	int64_t GetId();
	int64_t GetHp();
	int64_t GetSpeed();
	int64_t GetAttackTargetId();
	physics::Vector2D GetVelocity();
	physics::Vector2D GetPosition();
};


class STATE_EXPORT StateView {
private:
	std::shared_ptr<State> state;
public:
	std::vector<int64_t> GetUnitIds();
	std::vector<int64_t> GetEnemyIds();
	std::vector<int64_t> GetTowerIds();
	std::vector<int64_t> GetFlagIds();
	TerrainElementView XYToTerrainElement();
	TerrainElementView OffsetToTerrainElement();
	Actor
};


class STATE_EXPORT PlayerStateHandler {
private:
	int64_t player_id;
public:
	PlayerStateHandler();
	void MoveUnits();
	void AttackUnit();
	int64_t PlanPath();
};

}

#endif
