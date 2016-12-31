#include "actor/base.h"
#include "actor/flag.h"
#include "actor/king.h"
#include "actor/swordsman.h"
#include "bits/stdc++.h"
#include "json.hpp" //https://github.com/nlohmann/json
#include "path_planner/graph.h"
#include "path_planner/path_planner.h"
#include "path_planner/path_planner_helper.h"
#include "state.h"
#include "terrain/terrain.h"
#include "terrain/terrain_element.h"
#include <unistd.h>

using namespace std;
using namespace state;
using namespace physics;

using json = nlohmann::json;

void PrintVector(Vector2D v)
{
	cout << "(" << v.x << ", " << v.y << ") " << endl;
}

class MyFormation : public FormationMaker {
public:
	// int64_t formation_id;
	MyFormation()
		: FormationMaker()
	{
	}
	vector<Vector2D> ReturnFormation(int64_t formation_size)
	{
		vector<Vector2D> f(formation_size);
		for (int i = 0; i < formation_size; i++)
			f[i] = Vector2D(i * 20, 0);
		return f;
	}
};

void PrintTerrainElement(TerrainElement t)
{
	cout << "Terrain Element: \n";
	cout << "\tSize         : " << t.GetSize() << endl;
	cout << "\tPosition     : ";
	PrintVector(t.GetPosition());
	cout << "\tTerrain type : " << t.GetTerrainType() << endl;
	cout << endl;
}

void PrintTerrain(Terrain t)
{
	cout << "Terrain: \n";
	cout << "Size   : ";
	PrintVector(t.GetSize());

	auto te = t.CoordinateToTerrainElement(Vector2D(120, 22));
	PrintTerrainElement(te);
	te = t.OffsetToTerrainElement(Vector2D(3, 2));
	PrintTerrainElement(te);

	for (auto neighbour : t.GetAllNeighbours(Vector2D(1, 1)))
		cout << neighbour.x << neighbour.y << ", ";
	cout << endl;
	for (auto neighbour : t.GetAllNeighbours(Vector2D(1, 0), 1))
		cout << neighbour.x << neighbour.y << ", ";
	cout << endl;
}

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

const int te_size = 20;
int main(int argc, char const* argv[])
{
	TerrainElement t(PLAIN, Vector2D(22, 11), te_size);
	// PrintTerrainElement(t);
	Terrain T(10);

	vector<vector<TerrainElement> > grid;
	for (int i = 0; i < 20; i++) {
		vector<TerrainElement> temp;
		for (int j = 0; j < 20; j++)
			temp.push_back(
				TerrainElement(PLAIN, Vector2D(i * te_size, j * te_size), te_size));
		grid.push_back(temp);
	}
	Terrain TT(grid);

	vector<shared_ptr<Actor> > actors;
	vector<shared_ptr<Actor> > temp;
	vector<vector<shared_ptr<Actor> > > sorted_actors;

	// actors[0] -> base
	// actors[1] -> flag
	// actors[2] -> king

	vector<shared_ptr<Base> > bases(1);
	vector<shared_ptr<Flag> > flags(2);
	vector<shared_ptr<King> > kings(1);
	bases[0] = shared_ptr<Base>(new Base(0, PLAYER1, 10, 10, 10, 10, 10, 10, 0, 0, Vector2D(0, 0), Vector2D(0, 0), 2));
	flags[0] = shared_ptr<Flag>(new Flag(1, PLAYER1, 10, 10, 10, 10, 10, 10, 0, 0, Vector2D(0, 0), Vector2D(0, 0), 2));
	flags[1] = shared_ptr<Flag>(new Flag(2, PLAYER2, 10, 10, 10, 10, 10, 10, 0, 0, Vector2D(200, 200), Vector2D(0, 0), 2));
	kings[0] = shared_ptr<King>(new King(3, PLAYER1, 10, 10, 10, 10, 10, 10, 0, 0, Vector2D(150, 150), Vector2D(0, 0), 2));
	AddActor(actors, bases[0]);
	AddActor(actors, flags[0]);
	AddActor(temp, flags[1]);
	AddActor(actors, kings[0]);

	for (int i = 4; i <= 7; i++) {
		Actor* actor = new Swordsman(i, PLAYER1, 10, 10, 10, 10, 10, 10, 0, 0,
			Vector2D(0, 0), Vector2D(0, 0), 2);
		AddActor(actors, actor);
	}
	sorted_actors.push_back(actors);

	for (int i = 8; i <= 11; i++) {
		Actor* actor = new Swordsman(i, PLAYER2, 10, 10, 10, 10, 10, 10, 0, 0,
			Vector2D(0, 0), Vector2D(0, 0), 2);
		AddActor(temp, actor);
	}
	sorted_actors.push_back(temp);

	State S(TT, sorted_actors, kings, bases, flags);
	shared_ptr<FormationMaker> F(new MyFormation());
	vector<int64_t> weights({ 100, 200, 300 });

	S.Update(1);

	auto e = S.GetPlayerEnemyIds(PLAYER2);
	assert(e == list_act_id_t({0,1,4,5,6,7}));
	// Expect: 0,1,4,5,6,7
	// 3 -> too far away

	// for(auto i: e) {
	//     cout << i << ',';
	// }
	// cout << endl;

	e = S.GetActorEnemies(PLAYER1, 6);
	assert(e == list_act_id_t({8,9,10,11}));

	S.MoveUnits(list_act_id_t({ 6 }), Vector2D(9, 9), F, weights);

	S.Update(40);    // 6 moves out of los
	e = S.GetPlayerEnemyIds(PLAYER2);
	assert(e == list_act_id_t({0,1,4,5,7}));

	e = S.GetActorEnemies(PLAYER1, 6);
	assert(e == list_act_id_t({}));
	cout << "success" << endl;
	return 0;

	// int c = 0;
	// for (int i = 0;; i++) {
	//     // freopen("output.txt", "w", stdout);
	//     switch (c) {
	//     case 0:
	//         if (!S.GetKing(PLAYER1)->GetPathPlannerHelper()->IsPathPlanning()) {
	//             S.FlagCapture(PLAYER1, NULL);
	//             S.MoveUnits(list_act_id_t({ 3 }), bases[PLAYER1]->GetPosition(), F, weights);
	//             c = 1;
	//         }
	//         break;
	//     case 1:
	//         if (!S.GetKing(PLAYER1)->GetPathPlannerHelper()->IsPathPlanning()) {
	//             S.FlagDrop(PLAYER1, NULL);
	//             S.MoveUnits(list_act_id_t({ 3 }), Vector2D(5,5), F, weights);
	//             c = 2;
	//         }
	//         break;
	//     case 2:
	//         if (!S.GetKing(PLAYER1)->GetPathPlannerHelper()->IsPathPlanning()) {
	//             c = 3;
	//         }
	//         break;
	//     }
	//     // cout << c << endl;
	//     if (c == 3)
	//         break;
	//     ofstream file("output.txt");
	//     S.Update(1);
	//     json arr = json::array();
	//     for (auto actor : actors) {
	//         json j;
	//         auto pos = actor->GetPosition();
	//         j["id"] = actor->GetId();
	//         j["x"] = pos.x;
	//         j["y"] = pos.y;
	//         arr.push_back(j);
	//     }
	//     file << arr.dump() << endl;
	//     file.flush();
	//     this_thread::sleep_for(chrono::milliseconds(20));
	// }

	return 0;
}
