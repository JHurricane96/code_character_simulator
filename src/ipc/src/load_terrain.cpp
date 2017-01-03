/**
 * @file load_terrain.cpp
 * Function definitions for retrieving and loading terrain
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "state.h"
#include "utilities.h"
#include "actor/actor.h"
#include "ipc.h"
#include "state.pb.h"

using namespace std;
using namespace state;
using namespace physics;

/**
 * Populates the terrain
 *
 * Terrain is a grid where each unit (terrain element) consists of position
 * size, terrain type, line of sight for a given player and timestamp denoting
 * last visit by the players
 *
 * @param[in]  StateVar        the state object
 * @param[in]  TerrainMessage  the terrain message object
 *
 * @return     Exit status
 */
int PopulateTerrain(IPC::Terrain* TerrainMessage, std::shared_ptr<state::State>* StateVar) {

	/**
	 * Iterator to traverse the terrain grid by row
	 */
	std::vector< std::vector<TerrainElement> >::iterator row;
	/**
	 * Iterator to traverse the terrain grid's row by column
	 */
	std::vector<TerrainElement>::iterator col;

	/**
	 * 2D vector to store the terrain grid
	 */
	std::vector<std::vector<TerrainElement> > terrain_grid = StateVar->terrain;

	/**
	 * Add a grid message
	 */
	IPC::TerrainRows* terrain_grid_message = TerrainMessage->add_grid();

	/**
	 * Loop through the grid to fill the terrain element messages unit by unit
	 */

	for (row = terrain_grid.begin(); row!= terrain_grid.end(); ++row) {
		for (col = row.begin(); col!= row.end(); ++col) {

			/**
			 * Vector2D to store and map coordinate to terrain element
			 */
			physics::Vector2D position, translated_position;
			/**
			 * To find x and y coordinates of the grid
			 */
			double x_value = std::distance(terrain_grid.begin(), row);
			double y_value = std::distance(row.begin(), col);

			position->x = x_value;
			position->y = y_value;

			/**
			 * Add a terrain element message in grid message
			 */
			IPC::TerrainElement* terrain_element_message = terrain_grid_message->add_terrain_element();

			state::TerrainElement terrain_element_object = terrain_grid->CoordinateToTerrainElement(position);

			translated_position = terrain_element_object->GetPosition();

			/**
			 * Set position of terrain element
			 */
			IPC::Vector2D element_position = terrain_element_message->set_allocated_position();
			element_position.set_x(translated_position.x);
			element_position.set_y(translated_position.y);
			/**
			 * Set size of terrain element
			 */
			terrain_element_message->set_size(terrain_element_object->GetSize());
			/**
			 * Set size of terrain element
			 */
			terrain_element_message->set_terrain_type(terrain_element_object->GetTerrainType());
			/**
			 * Set the line of sight for a given player
			 */
			terrain_element_message->set_los_type_player_1(terrain_element_object->GetLos(player_id));
			terrain_element_message->set_los_type_player_2(terrain_element_object->GetLos(player_id));
			/**
			 * Set the the timestamps noting when the element was last visited by
			 * the players
			 */
			terrain_element_message->set_last_seen_player_1(terrain_element_object->GetLastSeen(player_id));
			terrain_element_message->set_last_seen_player_2(terrain_element_object->GetLastSeen(player_id));
		}
	}

	return 0;
}