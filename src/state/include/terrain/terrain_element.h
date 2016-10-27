#ifndef STATE_TERRAIN_TERRAIN_ELEMENT_H
#define STATE_TERRAIN_TERRAIN_ELEMENT_H

#include <cstdint>
#include "point.h"
#include "state_export.h"

namespace state {

enum TERRAIN_TYPE {
	PLAIN,
	FOREST,
	MOUNTAIN,
	UNDEFINED,
};

enum LOS_TYPE {
	UNEXPLORED,
	EXPLORED,
	DIRECT_LOS,
};

class STATE_EXPORT TerrainElement {
private:
	point::Point position;
	int64_t size;
	TERRAIN_TYPE terrain_type;
	LOS_TYPE los_type_player_1;
	LOS_TYPE los_type_player_2;
	int64_t last_seen_player_1, last_seen_player_2;
public:
	TerrainElement(
		TERRAIN_TYPE terrain_type,
		point::Point position,
		int64_t size
	);
};

}

#endif
