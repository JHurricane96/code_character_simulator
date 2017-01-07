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
#include "state.pb.h"
#include "terrain.pb.h"

using namespace std;

const int te_size = 20;

state::Terrain DepopulateTerrain(const IPC::Terrain& RetrievedMessage) {

	vector<vector<state::TerrainElement> > Grid;

	//cout<<"This is the terrain speaking\n";
	for (int i = 0; i < RetrievedMessage.row_size(); i++)
	{
		const IPC::Terrain::TerrainRow& RowMessage = RetrievedMessage.row(i);
		vector<state::TerrainElement> Row;
		for(int j = 0; j < RowMessage.element_size(); j++)
		{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			const IPC::Terrain::TerrainElement& ElementMessage = RowMessage.element(j);
			IPC::Terrain::Vector2D Position(ElementMessage.position());
			//cout<<"Position: ("<<Position.x()<<","<<Position.y()<<") ";
			//cout<<"Size: "<<ElementMessage.size()<<" ";
			state::TERRAIN_TYPE terrain_type;
			switch(ElementMessage.type())
			{
				case IPC::Terrain::TerrainElement::PLAIN : 
					//cout<<"type: PLAIN\n";
					terrain_type = state::PLAIN;
					break;
				case IPC::Terrain::TerrainElement::FOREST : 
					//cout<<"type: FOREST\n";
					terrain_type = state::FOREST;
					break;
				case IPC::Terrain::TerrainElement::MOUNTAIN : 
					//cout<<"type: MOUNTAIN\n";
					terrain_type = state::MOUNTAIN;
					break;
			}
			Row.push_back(state::TerrainElement(terrain_type, physics::Vector2D(i * te_size, j * te_size), te_size));
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

		IPC::Terrain RetrievedMessage;

		fstream input("terrain_level1.txt", ios::in | ios::binary);

		if (!RetrievedMessage.ParseFromIstream(&input)) {
			cerr << "Failed to retrieve state message" << endl;
			return -1;
		}

		return ( DepopulateTerrain(RetrievedMessage) );
	}
}