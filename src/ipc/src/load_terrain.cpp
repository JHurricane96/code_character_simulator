/**
 * @file load_terrain.cpp
 * Function definitions for loading terrain from file
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "ipc.h"
#include "terrain/terrain.h"
#include "terrain/terrain_element.h"
#include "terrain.pb.h"

using namespace std;

const int TerrainSize = 20;

/**
 * Depopulates the terrain
 * 
 * Terrain is a grid where each unit (terrain element) consists of position
 * size, terrain type, line of sight for a given player and timestamp denoting
 * last visit by the players
 *
 * @param[in]  TerrainMessage  the IPC::Terrain message object
 * @param[out] TerrainObject   the state::Terrain object 
 */
state::Terrain DepopulateTerrain(const IPC::Terrain& TerrainMessage) {

	vector<vector<state::TerrainElement> > Grid;

	/**
	 * Iterates through the TerrainMessage object and fills in the Grid with
	 * position, size and type of terrain
	 */
	for (int i = 0; i < TerrainMessage.row_size(); i++)
	{
		const IPC::Terrain::TerrainRow& RowMessage = TerrainMessage.row(i);
		vector<state::TerrainElement> Row;
		for(int j = 0; j < RowMessage.element_size(); j++)
		{

			const IPC::Terrain::TerrainElement& ElementMessage = RowMessage.element(j);

			IPC::Terrain::Vector2D Position(ElementMessage.position());
			
			state::TERRAIN_TYPE terrain_type;
			switch(ElementMessage.type())
			{
				case IPC::Terrain::TerrainElement::PLAIN : 
					terrain_type = state::PLAIN;
					break;
				case IPC::Terrain::TerrainElement::FOREST : 
					terrain_type = state::FOREST;
					break;
				case IPC::Terrain::TerrainElement::MOUNTAIN : 
					terrain_type = state::MOUNTAIN;
					break;
			}
			Row.push_back(state::TerrainElement(terrain_type, physics::Vector2D(i * TerrainSize, j * TerrainSize), TerrainSize));
		}
		Grid.push_back(Row);
	}
	state::Terrain TerrainObject(Grid);

	return TerrainObject;
}

namespace ipc {

	/**
	 * Loads the terrain from a file
	 *
	 * @param[in]  Terrain  the terrain object
	 *
	 * @return     Exit status
	 */

	state::Terrain LoadTerrain() {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Terrain TerrainMessage;

		fstream input("terrain_level1.txt", ios::in | ios::binary);

		if (!TerrainMessage.ParseFromIstream(&input)) {
			cerr << "Failed to retrieve state message" << endl;
			return -1;
		}

		return ( DepopulateTerrain(TerrainMessage) );
	}
}