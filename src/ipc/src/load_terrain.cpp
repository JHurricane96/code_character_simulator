/**
 * @file load_terrain.cpp
 * Function definitions for retrieving and loading terrain
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "state.h"
#include "terrain/terrain.h"
#include "utilities.h"
#include "actor/actor.h"
#include "ipc.h"
#include "terrain.pb.h"

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
 * @param[in]  TerrainVar      the terrain object
 * @param[in]  TerrainMessage  the terrain message object
 *
 * @return     Exit status
 */
int PopulateTerrain(state::Terrain TerrainVar, IPC::Terrain* TerrainMessage) {

	int64_t size = TerrainVar.GetRows();

	/**
	 * Loop through the terrain to fill the terrain element unit by unit
	 */

	for (double row = 0; row < size; ++row) {

		/**
		 * Add a terrain row in terrain
		 */
		IPC::Terrain::TerrainRow* RowMessage = TerrainMessage->add_row();
		for (double col = 0; col < size; ++col) {

			/**
			 * Add a terrain element in row
			 */
			IPC::Terrain::TerrainElement* ElementMessage = RowMessage->add_element();

			/**
			 * Vector2D to store and map offset to terrain element
			 */
			physics::Vector2D offset;

			offset.x = row;
			offset.y = col;

			state::TerrainElement ElementObject = TerrainVar.OffsetToTerrainElement(offset);

			/**
			 * Get position of terrain element
			 */
			physics::Vector2D position = ElementObject.GetPosition();

			IPC::Terrain::Vector2D* ElementPosition(new IPC::Terrain::Vector2D);

			/**
			 * Set position in message
			 */
			ElementPosition->set_x(position.x);
			ElementPosition->set_y(position.y);
			ElementMessage->set_allocated_position(ElementPosition);

			/**
			 * Get size of terrain element
			 */
			int64_t ElementSize = ElementObject.GetSize();

			/**
			 * Set size in message
			 */
			ElementMessage->set_size(ElementSize);

			/**
			 * Get type of terrain element
			 */
			state::TERRAIN_TYPE terrain_type = ElementObject.GetTerrainType();

			/**
			 * Set type in message
			 */
			switch(terrain_type){
				case PLAIN :
					ElementMessage->set_type(IPC::Terrain::TerrainElement::PLAIN);
					break;
				case FOREST :
					ElementMessage->set_type(IPC::Terrain::TerrainElement::FOREST);
					break;
				case MOUNTAIN :
					ElementMessage->set_type(IPC::Terrain::TerrainElement::MOUNTAIN);
					break;
			}
		}
	}

	return 0;
}

namespace ipc {

	/**
	 * Loads/Stores the terrain from/to the file
	 *
	 * @param[in]  Terrain  the terrain object
	 *
	 * @return     Exit status
	 */

	int TerrainTransfer(state::Terrain TerrainVar) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Terrain TerrainMessage;

		fstream output("terrain_level1.txt", ios::out | ios::trunc | ios::binary);


		if (PopulateTerrain(TerrainVar, &TerrainMessage) < 0) {
			cerr << "Failed to convert terrain into message" << endl;
			return -1;
		}

		if (!TerrainMessage.SerializeToOstream(&output)) {
			cerr << "Failed to transfer terrain message to file" << endl;
			return -1;
		}

		return 0;
	}
}

