#ifndef STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H
#define STATE_PLAYER_STATE_HANDLER_PLAYER_STATE_HANDLER_H

#include <memory>
#include <cstdint>
#include "point.h"
#include "terrain/terrain_element.h"
#include "state_export.h"

namespace state {

struct TerrainElementView {
	point::Point position;
	int64_t size;
	TERRAIN_TYPE terrain_type;
	LOS_TYPE los_type;
};

class STATE_EXPORT PlayerStateHandler {
private:
	std::shared_ptr<State> state;
	int64_t player_id;
public:
	PlayerStateHandler();
	TerrainElementView XYToTerrainElement();
	TerrainElementView OffsetToTerrainElement();
	void MoveUnits();
	void AttackUnit();
	int64_t PlanPath();
};

}

#endif
