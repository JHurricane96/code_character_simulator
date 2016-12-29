#include <terrain/terrain_element.h>

namespace state {

TerrainElement::TerrainElement() :
	terrain_type(),
	position(),
	size(),
	los_type_player(std::vector<LOS_TYPE>(LAST_PLAYER + 1, UNEXPLORED)) {}

TerrainElement::TerrainElement(TERRAIN_TYPE terrain_type,
                               physics::Vector2D position,
                               int64_t size) :
	terrain_type(terrain_type),
	position(position),
	size(size),
	los_type_player(std::vector<LOS_TYPE>(LAST_PLAYER + 1, UNEXPLORED)) {}

int64_t TerrainElement::GetSize() {
	return size;
}

physics::Vector2D TerrainElement::GetPosition() {
	return position;
}

TERRAIN_TYPE TerrainElement::GetTerrainType() {
	return terrain_type;
}

LOS_TYPE TerrainElement::GetLos(int64_t player_id) {
	return los_type_player[player_id];
}

}
