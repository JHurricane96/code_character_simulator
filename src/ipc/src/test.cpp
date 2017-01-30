#include "actor/swordsman.h"
#include "actor/tower.h"
#include "actor/arrow.h"
#include "actor/archer.h"
#include "bits/stdc++.h"
#include "path_planner/graph.h"
#include "path_planner/path_planner.h"
#include "path_planner/path_planner_helper.h"
#include "state.h"
#include "terrain/terrain.h"
#include "terrain/terrain_element.h"
#include "ipc.h"
#include <unistd.h>
#include <thread>

using namespace std;
using namespace state;
using namespace physics;
using namespace ipc;

const int te_size = 20;

class MyFormation : public FormationMaker {
public:
	MyFormation()
		: FormationMaker()
	{
	}
	vector<Vector2D> ReturnFormation(int64_t formation_size)
	{
		vector<Vector2D> f(formation_size);
		for (int i = 0; i < formation_size; i++)
			f[i] = Vector2D(i * 30, i);
		return f;
	}
};

void AddActor(vector<shared_ptr<Actor> >& actors, Actor* actor)
{
	shared_ptr<Actor> act(actor);
	act->AddPathPlanner(PathPlannerHelper(act));
	actors.push_back(act);
}

void AddActor(vector<shared_ptr<Actor> >& actors, shared_ptr<Actor> actor)
{
	actor->AddPathPlanner(PathPlannerHelper(actor));
	actors.push_back(actor);
}

string ActorTypeToString(ActorType type) {
	switch(type) {
		case ActorType::ARCHER		: return "archer";
		case ActorType::ARROW		: return "arrow";
		case ActorType::BASE		: return "base";
		case ActorType::FLAG		: return "flag";
		case ActorType::KING		: return "king";
		case ActorType::SCOUT		: return "scout";
		case ActorType::SWORDSMAN	: return "sword";
		case ActorType::TOWER		: return "tower";
	}
}

pair<State, vector<vector<shared_ptr<Actor> > > > MakeState(Terrain terrain) {
	vector<std::shared_ptr<Actor> > actors1;
	vector<std::shared_ptr<Actor> > actors2;
	vector<std::shared_ptr<Actor> > arch;
	vector<vector<shared_ptr<Actor> > > sorted_actors;
	vector<vector<shared_ptr<Swordsman> > > swordsmen;
	vector<shared_ptr<Swordsman> > sw;

	vector<shared_ptr<Base> > bases(2);
	vector<shared_ptr<Flag> > flags(2);
	vector<shared_ptr<King> > kings(2);

	for (int i = 6; i < 26; i++) {
		if (i < 10) {
			Actor* actor = new Tower(i, PLAYER1, 50, 100, 100, 10, 10, 0, 0, 0, Vector2D(99*(10-i), 99*(10-i)), Vector2D(0, 0), 1000, 40, 100, 100, 1000, 20, 300, 10);
			AddActor(actors1, actor);
		}
		else if (i < 15) {
			Actor* actor = new Archer(i, PLAYER2, 50, 100, 100, 10, 10, 0, 0, 0, Vector2D(99, 9), Vector2D(0, 0), 1000, 10, 1000, 10, 300, 10);
			AddActor(arch, actor);
		} else {
			Actor* actor = new Swordsman(i, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 388), Vector2D(0, 0), 100, 10, 10);
			AddActor(actors2, actor);
			sw.push_back(static_pointer_cast<Swordsman>(actors2.back()));
		}
	}
	vector<std::shared_ptr<Tower> > t;
	vector<vector<shared_ptr<Tower> > > towers;

	for (auto i: actors1) {
		shared_ptr<Tower> derived = static_pointer_cast<Tower>(i);
		// cout << derived->GetId() << endl;
		t.push_back(derived);
	}

	towers.push_back(t);
	towers.push_back(vector<std::shared_ptr<Tower> >());

	vector<std::shared_ptr<Archer> > a;
	vector<vector<shared_ptr<Archer> > > archers;

	for (auto i: arch) {
		shared_ptr<Archer> derived = static_pointer_cast<Archer>(i);
		// cout << derived->GetId() << endl;
		a.push_back(derived);
	}

	archers.push_back(vector<std::shared_ptr<Archer> >());
	archers.push_back(a);
	actors2.insert(actors2.end(), arch.begin(), arch.end());
	for (int i = 26; i < 36; i++) {
		Actor* actor = new Swordsman(i, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 388), Vector2D(0, 0), 10, 10, 10);
		AddActor(actors1, actor);
		sw.push_back(static_pointer_cast<Swordsman>(actors1.back()));
	}

	swordsmen.push_back(vector<shared_ptr<Swordsman> >());
	swordsmen.push_back(sw);

	flags[0] = shared_ptr<Flag>(new Flag(0, PLAYER1, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 0), Vector2D(0, 0), 2, 10));
	flags[1] = shared_ptr<Flag>(new Flag(1, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(100, 300), Vector2D(0, 0), 5, 10));

	kings[0] = shared_ptr<King>(new King(2, PLAYER1, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(200, 20), Vector2D(0, 0), 2, 10));
	kings[1] = shared_ptr<King>(new King(3, PLAYER2, 10, 1000, 1000, 10, 10, 10, 0, 0, Vector2D(399, 10), Vector2D(0, 0), 2, 10));

	bases[0] = shared_ptr<Base>(new Base(4, PLAYER1, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(0, 0), Vector2D(0, 0), 2, 10, 12, 2));
	bases[1] = shared_ptr<Base>(new Base(5, PLAYER2, 10, 100, 100, 10, 10, 10, 0, 0, Vector2D(100, 100), Vector2D(0, 0), 2, 10, 12, 2));

	AddActor(actors1, flags[0]);
	AddActor(actors1, kings[0]);
	AddActor(actors1, bases[0]);
	AddActor(actors2, flags[1]);
	AddActor(actors2, kings[1]);
	AddActor(actors2, bases[1]);

	sorted_actors.push_back(actors1);
	sorted_actors.push_back(actors2);

	//cout << "SIZE: " << archers[1].size() << "\n";

	State S(
		terrain,
		sorted_actors,
		kings,
		bases,
		flags,
		towers,
		vector<vector<shared_ptr<Scout> > >(2, vector<shared_ptr<Scout> >()),
		archers,
		swordsmen
	);
	return make_pair(S, sorted_actors);
}

vector<shared_ptr<Actor> > GetFlatActors(vector<vector<shared_ptr<Actor> > > sorted_actors) {
	auto actors = flatten(sorted_actors);
	std::sort(actors.begin(), actors.end(),
		[](const std::shared_ptr<Actor>& a1,
			const std::shared_ptr<Actor>& a2) {
			return a1->GetId() < a2->GetId();
		});
	return actors;
}

void PrintLos(Terrain terrain, PlayerId player_id) {
	for(int i = 0; i < terrain.GetRows(); i++) {
		for(int j = 0; j < terrain.GetRows(); j++) {
			cout << terrain.OffsetToTerrainElement(Vector2D(i,j)).GetLos(player_id) << ' ';
		}
		cout << endl;
	}
}

int main(int argc, char const* argv[])
{
	Terrain T(10);

	vector<vector<TerrainElement> > grid;

	for (int i = 0; i < 20; i++) {
		vector<TerrainElement> temp;
		for (int j = 0; j < 20; j++)
			if (j <= 10)
			{
				temp.push_back(TerrainElement(PLAIN, Vector2D(i * te_size, j * te_size), te_size));
				//cout<<"i: "<<i*te_size<<" j: "<<j*te_size<<endl;
				state::TERRAIN_TYPE terrain_type = temp[j].GetTerrainType();
				switch(terrain_type){
				case PLAIN :
					//cout<<"PLAIN\n";
					break;
				case FOREST :
					//cout<<"FOREST\n";
					break;
				case MOUNTAIN :
					//cout<<"MOUNTAIN\n";
					break;
			}
							}
			else
				temp.push_back(TerrainElement(MOUNTAIN, Vector2D(i * te_size, j * te_size), te_size));
		grid.push_back(temp);
	}

	Terrain TT(grid);

	shared_ptr<FormationMaker> F(new MyFormation());

	vector<int64_t> weights({ 100, 200, 300 });
	list_act_id_t l({ 6, 7 });

	list_act_id_t s({ 13, 12 });

	auto thing = MakeState(TT);
	auto S = shared_ptr<State>(new State(thing.first));
	auto sorted_actors = thing.second;
	auto actors = GetFlatActors(sorted_actors);

	int bleep = 0;
	double i=0.0001;

	//while (true) {

		//auto start_time = chrono::high_resolution_clock::now();
		//ofstream file("output.txt");
		//::StoreTerrain(TT);
		//auto arrows = S->GetProjectiles();
		//Terrain ter = ipc::LoadTerrain("terrain_level2.txt");
		Interrupts* InterruptVar(new Interrupts);
		std::thread RendererInput(ipc::IncomingInterrupts, InterruptVar);
		//Lol->SetPlayStatus(false);
		//InterruptTransfer(Lol);
		while(true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
			//cout<<"Here"<<endl;
			//ipc::IncomingInterrupts(Lol);
			/*bool play = InterruptVar->GetPlayStatus();
			if(play==false)
				cout<<"false"<<endl;
			else
				cout<<"true"<<endl;
			int level = InterruptVar->GetLevelNumber();
			cout<<level<<endl;
			bool exit = InterruptVar->GetExitStatus();
			if(exit==false)
				cout<<"false"<<endl;
			else
				cout<<"true"<<endl;
			bool restart = InterruptVar->GetRestartStatus();
			if(restart==false)
				cout<<"false"<<endl;
			else
				cout<<"true"<<endl;*/
			ipc::StateTransfer(S);
			i+=0.0001;
		}
		RendererInput.join();
		/*int64_t ter_size = ter.GetRows();

		for (double ii = 0; ii < ter_size; ii++) {
			for (double jj = 0; jj < ter_size; jj++){
				std::this_thread::sleep_for(std::chrono::milliseconds(30));

				physics::Vector2D ele_pos;
				ele_pos.x=ii;
				ele_pos.y=jj;

				state::TerrainElement ele = ter.OffsetToTerrainElement(ele_pos);
				physics::Vector2D ele_position = ele.GetPosition();
				int64_t ele_size = ele.GetSize();
				state::TERRAIN_TYPE ele_type = ele.GetTerrainType();

				cout<<"Position: ("<<ele_position.x<<","<<ele_position.y<<") ";
				cout<<"Size: "<<ele_size<<" ";
				switch(ele_type) {

					case state::PLAIN : 
						cout<<"type: PLAIN\n";
						break;
					case state::FOREST : 
						cout<<"type: FOREST\n";
						break;
					case state::MOUNTAIN : 
						cout<<"type: MOUNTAIN\n";
						break;
				}
			}
		}*/
		//ipc::StoreTerrain(TT);
		//double i=0.0001;
		//while(true){
			/*if( ipc::StateTransfer(S) < 0 ) {
			return -1;
			}*/
			//std::cout.flush();
			//i+=0.0001;
		//}

		//RendererInput.join();


		//file << arr.dump() << endl;
		//file.flush();
		//auto end_time = chrono::high_resolution_clock::now();
		//auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
		// cout << duration << endl;
		//this_thread::sleep_for(chrono::milliseconds(max((long) 0, 30 - duration)));
		//bleep += 30;
		//if (bleep / 1000 >= 15) {
			//break;
		//}
	//}

	return 0;
}
