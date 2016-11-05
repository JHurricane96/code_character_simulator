/**
 * @file terrain.h
 * Defines the Terrain class
 */

#ifndef STATE_TERRAIN_TERRAIN_H
#define STATE_TERRAIN_TERRAIN_H

#include <vector>
#include "terrain/terrain_element.h"
#include "state_export.h"

namespace state {
/**
 * Class for the entire terrain
 */
class STATE_EXPORT Terrain {
private:
	/**
	 * A 2D matrix of TerrainElements
	 */
	std::vector<std::vector<TerrainElement> > grid;
public:
	Terrain();
	/**
	 * Gets TerrainElement corresponding to position vector
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElement XYToTerrainElement();
	/**
	 * Gets TerrainElement corresponding to position offset
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElement OffsetToTerrainElement();
};

}

#endif
