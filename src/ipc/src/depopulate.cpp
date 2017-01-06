/**
 * @file depopulate.cc
 * Function definitions for data transfer class functions
*/
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "ipc.h"
#include "state.pb.h"
#include "terrain.pb.h"

using namespace std;


int DepopulateActors(const IPC::State& RetrievedMessage) {

	for (int i = 0; i < RetrievedMessage.actors_size(); i++)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(1));

		cout<<"In here"<<endl;
		const IPC::State::Actor& ActorMessage = RetrievedMessage.actors(i);

		cout<<"Actor ID: "<<ActorMessage.id()<<endl;
		cout<<"Actor PlayerID: "<<ActorMessage.player_id()<<endl;
		cout<<"Actor PosX: "<<ActorMessage.pos_x()<<endl;
		cout<<"Actor PosY: "<<ActorMessage.pos_y()<<endl;
		
		if(ActorMessage.attack() == false)
			cout<<"Actor attack target: False"<<endl;
		else
			cout<<"Actor attack target: True"<<endl;
		
		cout<<"Actor HP: "<<ActorMessage.hp()<<endl;
		cout<<"Actor Max HP: "<<ActorMessage.max_hp()<<endl;
	}

	return 0;
}
int DepopulateTerrain(const IPC::Terrain& RetrievedMessage) {

	cout<<"This is the terrain speaking\n";
	for (int i = 0; i < RetrievedMessage.row_size(); i++)
	{
		const IPC::Terrain::TerrainRow& RowMessage = RetrievedMessage.row(i);
		for(int j = 0; j < RowMessage.element_size(); j++)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			const IPC::Terrain::TerrainElement& ElementMessage = RowMessage.element(j);
			IPC::Terrain::Vector2D Position(ElementMessage.position());
			cout<<"Position: ("<<Position.x()<<","<<Position.y()<<") ";
			cout<<"Size: "<<ElementMessage.size()<<" ";
			switch(ElementMessage.type())
			{
				case IPC::Terrain::TerrainElement::PLAIN : 
					cout<<"type: PLAIN\n";
					break;
				case IPC::Terrain::TerrainElement::FOREST : 
					cout<<"type: FOREST\n";
					break;
				case IPC::Terrain::TerrainElement::MOUNTAIN : 
					cout<<"type: MOUNTAIN\n";
					break;
			}
		}
	}

	return 0;
}

namespace ipc {

	/**
	 * Depopulates the incoming interrupts
	 *
	 * Interrupts could be game level, play/pause state or console logs
	 *
	 * @return     Exit status
	 */

	int IncomingInterrupts() {

		/**
		 * Verify that the version of the library that we linked against is
		 * compatible with the version of the headers we compiled against
		 */
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		IPC::Terrain RetrievedMessage;

		fstream input("terrain_level1.txt", ios::in | ios::binary);
		istream *in = &cin;

		if (!RetrievedMessage.ParseFromIstream(&input)) {
			cerr << "Failed to retrieve state message" << endl;
			return -1;
		}

		if (DepopulateTerrain(RetrievedMessage) < 0) {
			cerr << "Failed to load actors from file" << endl;
			return -1;
		}

		return 0;
	}
}
