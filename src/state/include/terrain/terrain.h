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
	/**
	 * A helper vector that holds offsets to adjacent grid neighbours
	 */
	std::vector<physics::Vector2D> adj_neighbours;
	/**
	 * A helper vector that holds offsets to diagonal grid neighbours
	 */
	std::vector<physics::Vector2D> dia_neighbours;

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
	/**
	 * Gets the adjacent neighbours of a given TerrainElement
	 *
	 * @param[in]  offset  The grid offsets of the TerrainElement
	 *                     whose neighbours are needed
	 *
	 * @return     The adjacent neighbours
	 */
	std::vector<physics::Vector2D> GetAdjacentNeighbours(physics::Vector2D offset);
	/**
	 * Gets the diagonal neighbours of a given TerrainElement
	 *
	 * @param[in]  offset  The grid offset of the TerrainElement
	 *                     whose neighbours are needed
	 *
	 * @return     The diagonal neighbours
	 */
	std::vector<physics::Vector2D> GetDiagonalNeighbours(physics::Vector2D offset);
	/**
	 * Gets all the neighbours of a given TerrainElement
	 *
	 * @param[in]  offset  The grid offset of the TerrainElement
	 *                     whose neighbours are needed
	 *
	 * @return     The neighbours
	 */
	std::vector<physics::Vector2D> GetAllNeighbours(physics::Vector2D offset);

};

}

#endif
