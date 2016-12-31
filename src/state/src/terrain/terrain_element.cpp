#include <terrain/terrain_element.h>

namespace state {

TerrainElement::TerrainElement() :
	terrain_type(),
	position(),
	size(),
	los_type_player(std::vector<LOS_TYPE>(UNEXPLORED, UNEXPLORED)) {}

TerrainElement::TerrainElement(TERRAIN_TYPE terrain_type,
                               physics::Vector2D position,
                               int64_t size) :
	terrain_type(terrain_type),
	position(position),
	size(size),
	los_type_player(std::vector<LOS_TYPE>(UNEXPLORED, UNEXPLORED)) {}

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

void TerrainElement::SetLos(LOS_TYPE los, int64_t player_id) {
	los_type_player[player_id] = los;
}

list_act_id_t TerrainElement::GetUnits(PlayerId player_id) {
	return units[player_id];
}

void TerrainElement::SetUnits(
	PlayerId player_id,
	list_act_id_t new_units
) {
	units[player_id] = new_units;
}

void TerrainElement::AddUnit(
	PlayerId player_id,
	act_id_t new_unit
) {
	units[player_id].push_back(new_unit);
}

}
