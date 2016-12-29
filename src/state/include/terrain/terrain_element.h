/**
 * @file terrain_element.h
 * Defines the Terrain Element class
 */

#ifndef STATE_TERRAIN_TERRAIN_ELEMENT_H
#define STATE_TERRAIN_TERRAIN_ELEMENT_H

#include <cstdint>
#include <vector>
#include "vector2d.h"
#include "state_export.h"
#include "utilities.h"

namespace state {

/**
 * There are 3 terrain types Plains, Forests and Mountains
 * The Undefined type is to fill for terrain elements not yet visited
 * by the player
 */
enum TERRAIN_TYPE {
	PLAIN = 0,
	FOREST,
	MOUNTAIN,
	UNDEFINED,
};

/**
 * Describes the line of sight types
 * Unexplored : The player hasn't been here ever
 * Explored   : The player has visited this element but it is not in
 *              his direct LOS now
 * Direct LOS : In the player's LOS now
 */
enum LOS_TYPE {
	UNEXPLORED,
	EXPLORED,
	DIRECT_LOS,
};

/**
 * Class for terrain element
 */
class STATE_EXPORT TerrainElement {
private:
	/**
	 * Position vector to the top-left of the element
	 */
	physics::Vector2D position;
	/**
	 * Side length of the element
	 */
	int64_t size;
	/**
	 * The type of terrain
	 */
	TERRAIN_TYPE terrain_type;
	/**
	 * The LOS type for players
	 */
	std::vector<LOS_TYPE> los_type_player;
	/**
	 * The timestamps noting when the element was last visited by the players
	 */
	std::vector<int64_t> last_seen_player;
public:
	TerrainElement();
	/**
	 * Constructor for the Terrain Element
	 *
	 * @param[in]  terrain_type  The terrain type
	 * @param[in]  position      The position vector
	 * @param[in]  size          The side length
	 */
	TerrainElement(
		TERRAIN_TYPE terrain_type,
		physics::Vector2D position,
		int64_t size
	);
	/**
	 * Gets the size of the TerrainElement
	 *
	 * @return     The size of the TerrainElement
	 */
	int64_t GetSize();
	/**
	 * Gets the position vector of the TerrainElement
	 *
	 * @return     The position vector
	 */
	physics::Vector2D GetPosition();
	/**
	 * Gets the terrain type of the TerrainElement
	 *
	 * @return     The terrain type
	 */
	TERRAIN_TYPE GetTerrainType();
	/**
	 * Gets the Line of Sight for a given player
	 *
	 * @param[in]  player_id  The id of the player
	 *
	 * @return     The LOS Type
	 */
	LOS_TYPE GetLos(PlayerId player_id);
	/**
	 * Sets the Line of Sight for a given player
	 *
	 * @param[in]  los        The los to be set
	 * @param[in]  player_id  The id of the player
	 */
	void SetLos(LOS_TYPE los, PlayerId player_id);
};

}

#endif
