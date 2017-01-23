/**
 * @file terrain.h
 * Defines the Terrain class
 */

#ifndef STATE_TERRAIN_TERRAIN_H
#define STATE_TERRAIN_TERRAIN_H

#include <vector>
#include <memory>
#include "actor/actor.h"
#include "terrain/terrain_element.h"
#include "state_export.h"

namespace state {

/**
 * LOS mutipliers for various terrain types
 * 0: PLAIN
 * 1: FOREST
 * 2: MOUNTAIN
 * Multiplier for mountain to plain would be multiplier[MOUNTAIN][PLAIN]
 */
const std::vector<std::vector<float>> Multiplier({
	{(float) 1, (float) 0.7, (float) 1.3 },
	{(float) 0.7, (float) 0.7, (float) 0.7 },
	{(float) 1.3, (float) 1, (float) 1 }
});

/**
 * An entry in the queue while flooding during LOS update
 */
struct LosListEntry {
	/**
	 * The TerrainElement offset
	 */
	physics::Vector2D offset;
	/**
	 * The weight assigned to the TerrainElement
	 */
	float score;
	/**
	 * Constructor for LosListEntry
	 *
	 * @param[in]  offset  The offset
	 * @param[in]  score   The score
	 */
	LosListEntry(physics::Vector2D offset, float score);
	/**
	 * Less than operator
	 *
	 * @param[in]  rhs   The right hand side
	 *
	 * @return     true if this entry is less, false otherwise
	 */
	bool operator<(const LosListEntry& rhs);
};

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
	std::vector<physics::Vector2D> adjacent_neighbours;
	/**
	 * A helper vector that holds offsets to diagonal grid neighbours
	 */
	std::vector<physics::Vector2D> diagonal_neighbours;
	/**
	 * Helper method to update los of grid elements inside a radius
	 *
	 * @param[in]  offset  The grid offset
	 * @param[in]  radius  The radius in offsets to flood
	 * @param[in]  los     The LOS_TYPE to update with
	 * @param[in]  pid     The PlayerId whose LOS is to be updated
	 */
	void UpdateLos(
		physics::Vector2D offset,
		int64_t radius,
		LOS_TYPE los,
		PlayerId pid
	);
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
	 * Gets the number of rows in the terrain
	 *
	 * @return     The number of rows
	 */
	int64_t GetRows();
	/**
	 * Gets the adjacent neighbours of a given TerrainElement
	 *
	 * @param[in]  offset  The grid offsets of the TerrainElement
	 *                     whose neighbours are needed
	 * @param[in]  width   The width from the edge of the map to be ignored
	 *                     value is zero by default
	 *
	 * @return     The adjacent neighbours
	 */
	std::vector<physics::Vector2D> GetAdjacentNeighbours(physics::Vector2D offset, int64_t width = 0);
	/**
	 * Gets the diagonal neighbours of a given TerrainElement
	 *
	 * @param[in]  offset  The grid offset of the TerrainElement
	 *                     whose neighbours are needed
	 * @param[in]  width   The width from the edge of the map to be ignored
	 *                     value is zero by default
	 *
	 * @return     The diagonal neighbours
	 */
	std::vector<physics::Vector2D> GetDiagonalNeighbours(physics::Vector2D offset, int64_t width = 0);
	/**
	 * Gets all the neighbours of a given TerrainElement
	 *
	 * @param[in]  offset  The grid offset of the TerrainElement
	 *                     whose neighbours are needed
	 * @param[in]  width   The width from the edge of the map to be ignored
	 *                     value is zero by default
	 *
	 * @return     The neighbours
	 */
	std::vector<physics::Vector2D> GetAllNeighbours(physics::Vector2D offset, int64_t width = 0);
	/**
	 * Updates the LOS of the TerrainElements
	 * Updates which units are on which TerrainElement
	 *
	 * @param[in]  actors  The actors in the game
	 */
	void Update(std::vector<std::vector<std::shared_ptr<Actor> > > actors);
	/**
	 * Merges this, a player state's Terrain, with the main state's
	 * Terrain
	 *
	 * @param[in]  terrain  The main state's Terrain
	 */
	void MergeWithMain(const Terrain& terrain);
};

}

#endif
