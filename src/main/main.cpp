#include "actor/fire_ball.h"
#include "actor/magician.h"
#include "actor/swordsman.h"
#include "actor/tower.h"
// #include "bits/stdc++.h"
#include "json.hpp" //https://github.com/nlohmann/json
#include "main_driver.h"
#include "path_planner/graph.h"
#include "path_planner/path_planner.h"
#include "path_planner/path_planner_helper.h"
#include "player1.h"
#include "player2.h"
#include "player_state_handler/player_state_handler.h"
#include "state.h"
#include "terrain/terrain.h"
#include "terrain/terrain_element.h"
#include <fstream>
// #include <unistd.h>

using namespace std;
using namespace state;
using namespace physics;
using namespace player;
using namespace player1;
using namespace player2;
using namespace drivers;

using json = nlohmann::json;
const int64_t ELEMENT_SIZE = 50;

class MyFormation : public FormationMaker {
public:
	MyFormation() : FormationMaker() {}
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

string ActorTypeToString(ActorType type)
{
	switch (type) {
	case ActorType::MAGICIAN:
		return "magician";
	case ActorType::FIREBALL:
		return "fire_ball";
	case ActorType::BASE:
		return "base";
	case ActorType::FLAG:
		return "flag";
	case ActorType::KING:
		return "king";
	case ActorType::SCOUT:
		return "scout";
	case ActorType::SWORDSMAN:
		return "sword";
	case ActorType::TOWER:
		return "tower";
	default:
		return "actor";
	}
}

pair<State, vector<vector<shared_ptr<Actor> > > > MakeState(Terrain terrain)
{
	vector<shared_ptr<Base> > bases(2);
	vector<shared_ptr<Flag> > flags(2);
	vector<shared_ptr<King> > kings(2);
	vector<vector<shared_ptr<Scout> > > scouts(
		2, vector<shared_ptr<Scout> >(1));
	vector<vector<shared_ptr<Tower> > > towers(
		2, vector<shared_ptr<Tower> >(3));
	vector<vector<shared_ptr<Swordsman> > > swordsmen(
		2, vector<shared_ptr<Swordsman> >(20));
	vector<vector<shared_ptr<Magician> > > magicians(
		2, vector<shared_ptr<Magician> >(10));
	vector<vector<shared_ptr<Actor> > > sorted_actors(2);

	flags[0] = shared_ptr<Flag>(new Flag(0, PLAYER1, 0, 0, 0, 0, 10, 0, 0, 0,
		Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), Vector2D(0, 0), 0, 0));
	sorted_actors[0].push_back(static_pointer_cast<Actor>(flags[0]));

	flags[1] = shared_ptr<Flag>(new Flag(1, PLAYER2, 0, 0, 0, 0, 10, 0, 0, 0,
		Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE), Vector2D(0, 0), 0, 0));
	sorted_actors[1].push_back(static_pointer_cast<Actor>(flags[1]));

	bases[0] = shared_ptr<Base>(new Base(4, PLAYER1, 0, 0, 0, 0, 10, 0, 0, 0,
		Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), Vector2D(0, 0), 5, 0,
		4 * ELEMENT_SIZE, 10));
	sorted_actors[0].push_back(static_pointer_cast<Actor>(bases[0]));

	bases[1] = shared_ptr<Base>(new Base(5, PLAYER2, 0, 0, 0, 0, 10, 0, 0, 0,
		Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE), Vector2D(0, 0), 5, 0,
		4 * ELEMENT_SIZE, 10));
	sorted_actors[1].push_back(static_pointer_cast<Actor>(bases[1]));

	kings[0] = shared_ptr<King>(new King(2, PLAYER1, 0, 400, 400, 10, 10, 100,
		0, 0, Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), Vector2D(0, 0), 1,
		0));
	kings[0]->AddPathPlanner(PathPlannerHelper(kings[0]));
	sorted_actors[0].push_back(static_pointer_cast<Actor>(kings[0]));

	kings[1] = shared_ptr<King>(new King(3, PLAYER2, 0, 400, 400, 10, 10, 100,
		0, 0, Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE), Vector2D(0, 0), 1,
		0));
	kings[1]->AddPathPlanner(PathPlannerHelper(kings[1]));
	sorted_actors[1].push_back(static_pointer_cast<Actor>(kings[1]));

	scouts[0][0] = shared_ptr<Scout>(new Scout(6, PLAYER1, 0, 300, 100, 37, 10,
		55, 0, 0, Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), Vector2D(0, 0),
		4, 0, 0));
	scouts[0][0]->AddPathPlanner(PathPlannerHelper(scouts[0][0]));
	sorted_actors[0].push_back(static_pointer_cast<Actor>(scouts[0][0]));

	scouts[1][0] = shared_ptr<Scout>(new Scout(7, PLAYER2, 0, 300, 100, 37, 10,
		55, 0, 0, Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE),
		Vector2D(0, 0), 4, 0, 0));
	scouts[1][0]->AddPathPlanner(PathPlannerHelper(scouts[1][0]));
	sorted_actors[1].push_back(static_pointer_cast<Actor>(scouts[1][0]));

	vector<physics::Vector2D> tower_pos{ Vector2D(11, 5), Vector2D(22, 8),
		Vector2D(7, 13), Vector2D(20, 26), Vector2D(24, 18), Vector2D(9, 23) };

	act_id_t id_count = 7;
	for (int64_t i = 0; i < 2; i++) {
		for (int64_t j = 0; j < 3; j++) {
			cout << tower_pos[i * 3 + j] * ELEMENT_SIZE << ',';
			PlayerId p = static_cast<PlayerId>(i);
			towers[i][j] = shared_ptr<Tower>(new Tower(++id_count, p, 80, 600,
				600, 0, 10, 0, 0, 0, tower_pos[i * 3 + j] * ELEMENT_SIZE,
				Vector2D(0, 0), 5, 40, 5 * ELEMENT_SIZE, 100, 7, 10, 300, 2));
			// towers[i][j]->AddPathPlanner(PathPlannerHelper(towers[i][j]));
			sorted_actors[i].push_back(
				static_pointer_cast<Actor>(towers[i][j]));
		}
	}
	cout << endl;

	for (int64_t i = 0; i < 2; i++) {
		for (int64_t j = 0; j < 20; j++) {
			int lol = i == 0 ? 0 : 30;
			PlayerId p = static_cast<PlayerId>(i);
			swordsmen[i][j] = shared_ptr<Swordsman>(
				new Swordsman(++id_count, p, 50, 200, 200, 20, 10, 20, 0, 0,
					Vector2D(lol * ELEMENT_SIZE, lol * ELEMENT_SIZE),
					Vector2D(0, 0), 2, 10, 10));
			swordsmen[i][j]->AddPathPlanner(PathPlannerHelper(swordsmen[i][j]));
			sorted_actors[i].push_back(
				static_pointer_cast<Actor>(swordsmen[i][j]));
		}
	}

	for (int64_t i = 0; i < 2; i++) {
		for (int64_t j = 0; j < 10; j++) {
			int lol = i == 0 ? 0 : 30;
			PlayerId p = static_cast<PlayerId>(i);
			magicians[i][j] = shared_ptr<Magician>(
				new Magician(++id_count, p, 100, 150, 150, 30, 10, 30, 0, 0,
					Vector2D(lol * ELEMENT_SIZE, lol * ELEMENT_SIZE),
					Vector2D(0, 0), 3, 25, 3 * ELEMENT_SIZE, 60, 100, 10));
			magicians[i][j]->AddPathPlanner(PathPlannerHelper(magicians[i][j]));
			sorted_actors[i].push_back(
				static_pointer_cast<Actor>(magicians[i][j]));
		}
	}

	State S(terrain, sorted_actors, kings, bases, flags, towers, scouts,
		magicians, swordsmen);
	return make_pair(S, sorted_actors);
}

void PrintTT(Terrain terrain)
{
	for (int i = 0; i < terrain.GetRows(); i++) {
		for (int j = 0; j < terrain.GetRows(); j++) {
			cout << terrain.OffsetToTerrainElement(Vector2D(i, j))
						.GetTerrainType()
				 << ' ';
		}
		cout << endl;
	}
}
void PrintLos(Terrain terrain, PlayerId player_id)
{
	for (int i = 0; i < terrain.GetRows(); i++) {
		for (int j = 0; j < terrain.GetRows(); j++) {
			cout << terrain.OffsetToTerrainElement(Vector2D(i, j))
						.GetLos(player_id)
				 << ' ';
		}
		cout << endl;
	}
}

Terrain LoadTerrain(string file_name)
{
	string line;
	getline(ifstream(file_name), line);
	int rows = (int)sqrt(line.size());
	int total = line.size();
	cout << "ROWS: " << rows << " TOTAL : " << total << endl;
	vector<vector<TerrainElement> > grid;
	for (int i = 0; i < rows; i++) {
		vector<TerrainElement> temp;
		for (int j = 0; j < rows; j++) {
			TERRAIN_TYPE t;
			switch (line[i]) {
			case 'F':
				t = FOREST;
				break;
			case 'M':
				t = MOUNTAIN;
				break;
			default:
				t = PLAIN;
			}
			temp.push_back(TerrainElement(
				t, Vector2D(i * ELEMENT_SIZE, j * ELEMENT_SIZE), ELEMENT_SIZE));
		}
		grid.push_back(temp);
	}
	return Terrain(grid);
}

vector<shared_ptr<Actor> > GetFlatActors(
	vector<vector<shared_ptr<Actor> > > sorted_actors)
{
	auto actors = flatten(sorted_actors);
	std::sort(actors.begin(), actors.end(),
		[](const std::shared_ptr<Actor>& a1, const std::shared_ptr<Actor>& a2) {
			return a1->GetId() < a2->GetId();
		});
	return actors;
}

int main()
{
	Terrain TT(LoadTerrain("grid.txt"));
	PrintTT(TT);
	shared_ptr<FormationMaker> F(new MyFormation());

	vector<int64_t> weights({ 100, 200, 300 });
	list_act_id_t l({ 6, 7 });

	list_act_id_t s({ 13, 12 });

	auto thing = MakeState(TT);
	auto S = shared_ptr<State>(new State(thing.first));
	auto sorted_actors = thing.second;
	auto actors = GetFlatActors(sorted_actors);

	auto thing1 = MakeState(TT);
	auto S1 = shared_ptr<State>(new State(thing1.first));
	PlayerStateHandler PSH1(S1.get(), PLAYER1);
	auto sorted_actors1 = thing1.second;
	auto actors1 = GetFlatActors(sorted_actors1);

	auto thing2 = MakeState(TT);
	auto S2 = shared_ptr<State>(new State(thing2.first));
	PlayerStateHandler PSH2(S2.get(), PLAYER2);
	auto sorted_actors2 = thing2.second;
	auto actors2 = GetFlatActors(sorted_actors2);

	// S for main, S1 for player 1, S2 for player 2
	// S.MoveUnits(l, Vector2D(12 * ELEMENT_SIZE, 9 * ELEMENT_SIZE), F,
	// weights);

	MainDriver driver(PlayerAi(shared_ptr<PlayerAiHelper>(new Player1())),
		PlayerAi(shared_ptr<PlayerAiHelper>(new Player2())), S, S1, S2);

	driver.Run();

	int bleep = 0;

	while (true) {
		// PrintLos(S2.GetTerrain(), PLAYER2);
		auto start_time = chrono::high_resolution_clock::now();
		ofstream file("output.txt");

		auto fire_balls = S->GetProjectiles();
		// cout << fire_balls.size() << endl;
		// for (auto fire_ball: fire_balls){
		// 	auto p = fire_ball->GetPosition();
		// 	cout << fire_ball->GetId() << p.x << p.y << endl;
		// }
		json arr = json::array();
		for (auto actor : actors) {
			json j;
			auto pos = actor->GetPosition();
			j["id"] = actor->GetId();
			j["playerID"] = actor->GetPlayerId();
			j["x"] = pos.x;
			j["y"] = pos.y;
			if (actor->GetAttackTarget() == NULL)
				j["attack"] = false;
			else
				j["attack"] = true;
			j["actorType"] = ActorTypeToString(actor->GetActorType());
			j["HP"] = actor->GetHp();
			j["maxHP"] = actor->GetMaxHp();
			arr.push_back(j);
		}
		for (auto actor : fire_balls) {
			json j;
			auto pos = actor->GetPosition();
			j["id"] = actor->GetId();
			j["playerID"] = actor->GetPlayerId();
			j["x"] = pos.x;
			j["y"] = pos.y;
			if (actor->GetAttackTarget() == NULL)
				j["attack"] = false;
			else
				j["attack"] = true;
			j["actorType"] = ActorTypeToString(actor->GetActorType());
			j["HP"] = actor->GetHp();
			j["maxHP"] = actor->GetMaxHp();
			arr.push_back(j);
		}

		file << arr.dump() << endl;
		file.flush();
		auto end_time = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(end_time
			- start_time).count();
		// cout << duration << endl;
		this_thread::sleep_for(
			chrono::milliseconds(max<long>((long)0, 30 - duration)));
		bleep += 30;
		if (bleep / 1000 >= 15) {
			break;
		}
	}

	driver.Stop();

	return 0;
}
