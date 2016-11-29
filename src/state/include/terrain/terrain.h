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
	 * No of terrain elements per row in the square grid
	 */
	int64_t row_size;
	/**
	 * A 2D matrix of TerrainElements
	 */
	std::vector<std::vector<TerrainElement> > grid;
public:
	Terrain(int64_t nrows);
	Terrain(std::vector<std::vector<TerrainElement> > grid);
	/**
	 * Gets TerrainElement corresponding to position vector
	 *
	 * @param[in]  position  The position vector
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElement CoordinateToTerrainElement(physics::Vector2D position);
	/**
	 * Gets TerrainElement corresponding to grid offset
	 *
	 * @param[in]  offset  The position vector containing the offsets
	 *                     offset.x = row_no, offset.y = col_no
	 *
	 * @return     Required Terrain Element
	 */
	TerrainElement OffsetToTerrainElement(physics::Vector2D offset);
	/**
	 * Returns a position vector to the bottom right of the grid
	 *
	 * @return     A 2d vector to the bottom right of the grid
	 */
	physics::Vector2D GetSize();

};

}

#endif
