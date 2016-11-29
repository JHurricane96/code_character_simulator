#include <terrain/terrain_element.h>

namespace state {

TerrainElement::TerrainElement() : terrain_type(), position(), size() {}

TerrainElement::TerrainElement(TERRAIN_TYPE terrain_type,
                               physics::Vector2D position, int64_t size)
	: terrain_type(terrain_type), position(position), size(size) {}

int64_t TerrainElement::GetSize() {
	return size;
}

physics::Vector2D TerrainElement::GetPosition() {
	return position;
}

}
