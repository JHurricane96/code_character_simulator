#ifndef STATE_TERRAIN_TERRAIN_H
#define STATE_TERRAIN_TERRAIN_H

#include <vector>
#include "terrain/terrain_element.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Terrain {
private:
	std::vector<std::vector<TerrainElement> > grid;
public:
	Terrain();
	XYToTerrainElement();
	OffsetToTerrainElement();
};

}

#endif
