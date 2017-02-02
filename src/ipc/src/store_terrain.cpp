/**
 * @file store_terrain.cpp
 * Function definitions for storing terrain into file
*/
#include <iostream>
#include <string>
#include <fstream>
#include "state.h"
#include "terrain/terrain.h"
#include "ipc.h"
#include "terrain.pb.h"

/**
 * Populates the terrain
 *
 * Terrain is a grid where each unit (terrain element) consists of position
 * size, terrain type, line of sight for a given player and timestamp denoting
 * last visit by the players
 *
 * @param[in]  TerrainVar      the state::Terrain object
 * @param[in]  TerrainMessage  the ipc::Terrain message object
 */
void PopulateTerrain(state::Terrain TerrainVar, IPC::Terrain* TerrainMessage) {

	int64_t no_of_rows = TerrainVar.GetRows();

	/**
	 * Loop through the terrain to fill the terrain element unit by unit
	 */

	for (double row = 0; row < no_of_rows; ++row) {

		IPC::Terrain::TerrainRow* RowMessage = TerrainMessage->add_row();
		for (double col = 0; col < no_of_rows; ++col) {

			IPC::Terrain::TerrainElement* ElementMessage = RowMessage->add_element();

			physics::Vector2D offset;

			offset.x = row;
			offset.y = col;

			state::TerrainElement ElementObject = TerrainVar.OffsetToTerrainElement(offset);

			if(row == 0 && col == 0) {

				TerrainMessage->set_size_of_element(ElementObject.GetSize());
			}

			state::TERRAIN_TYPE terrain_type = ElementObject.GetTerrainType();

			switch(terrain_type){
				case state::PLAIN :
					ElementMessage->set_type(IPC::Terrain::TerrainElement::PLAIN);
					break;
				case state::FOREST :
					ElementMessage->set_type(IPC::Terrain::TerrainElement::FOREST);
					break;
				case state::MOUNTAIN :
					ElementMessage->set_type(IPC::Terrain::TerrainElement::MOUNTAIN);
					break;
			}
		}
	}
	TerrainMessage->set_no_of_rows(no_of_rows);

	return;
}

namespace ipc {

	/**
	 * Stores the terrain in a file
	 *
	 * @param[in]  TerrainVar	the terrain object
	 * @param[in]  filename		the storage file
	 */

	void StoreTerrain(state::Terrain TerrainVar, std::string filename) {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Terrain TerrainMessage;

		std::fstream output(filename, std::ios::out | std::ios::trunc | std::ios::binary);

		PopulateTerrain(TerrainVar, &TerrainMessage);

		TerrainMessage.SerializeToOstream(&output);
	}
}
