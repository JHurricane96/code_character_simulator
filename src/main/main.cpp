#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "player_state_handler/player_state_handler.h"
#include "ipc.h"
#include "main_driver.h"
#include "player1.h"
#include "player2.h"
#include "ai.h"
#include "ai1.h"

const int64_t ELEMENT_SIZE = 200;

/**
 * Makes a state object with the given terrain
 *
 * @param[in]  terrain  The terrain
 *
 * @return     The state object
 */
state::State MakeState(state::Terrain terrain)
{
	std::vector<std::shared_ptr<state::Base> > bases(2);
	std::vector<std::shared_ptr<state::Flag> > flags(2);
	std::vector<std::shared_ptr<state::King> > kings(2);
	std::vector<std::vector<std::shared_ptr<state::Scout> > > scouts(
		2, std::vector<std::shared_ptr<state::Scout> >(1));
	std::vector<std::vector<std::shared_ptr<state::Tower> > > towers(
		2, std::vector<std::shared_ptr<state::Tower> >(3));
	std::vector<std::vector<std::shared_ptr<state::Swordsman> > > swordsmen(
		2, std::vector<std::shared_ptr<state::Swordsman> >(20));
	std::vector<std::vector<std::shared_ptr<state::Magician> > > magicians(
		2, std::vector<std::shared_ptr<state::Magician> >(10));
	std::vector<std::vector<std::shared_ptr<state::Actor> > > sorted_actors(2);

	flags[0] = std::shared_ptr<state::Flag>(new state::Flag(0, state::PLAYER1, 0, 0, 0, 0, 10, 0, 0, 0,
		physics::Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), physics::Vector2D(0, 0), 0, 0));
	sorted_actors[0].push_back(std::static_pointer_cast<state::Actor>(flags[0]));

	flags[1] = std::shared_ptr<state::Flag>(new state::Flag(1, state::PLAYER2, 0, 0, 0, 0, 10, 0, 0, 0,
		physics::Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE), physics::Vector2D(0, 0), 0, 0));
	sorted_actors[1].push_back(std::static_pointer_cast<state::Actor>(flags[1]));

	bases[0] = std::shared_ptr<state::Base>(new state::Base(4, state::PLAYER1, 0, 0, 0, 0, 10, 0, 0, 0,
		physics::Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), physics::Vector2D(0, 0), 3, 0,
		4 * ELEMENT_SIZE, 10));
	sorted_actors[0].push_back(std::static_pointer_cast<state::Actor>(bases[0]));

	bases[1] = std::shared_ptr<state::Base>(new state::Base(5, state::PLAYER2, 0, 0, 0, 0, 10, 0, 0, 0,
		physics::Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE), physics::Vector2D(0, 0), 3, 0,
		4 * ELEMENT_SIZE, 10));
	sorted_actors[1].push_back(std::static_pointer_cast<state::Actor>(bases[1]));

	kings[0] = std::shared_ptr<state::King>(new state::King(2, state::PLAYER1, 0, 400, 400, 10, 10, 210,
		0, 0, physics::Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), physics::Vector2D(0, 0), 1,
		0));
	kings[0]->AddPathPlanner(state::PathPlannerHelper(kings[0]));
	sorted_actors[0].push_back(std::static_pointer_cast<state::Actor>(kings[0]));

	kings[1] = std::shared_ptr<state::King>(new state::King(3, state::PLAYER2, 0, 400, 400, 10, 10, 210,
		0, 0, physics::Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE), physics::Vector2D(0, 0), 1,
		0));
	kings[1]->AddPathPlanner(state::PathPlannerHelper(kings[1]));
	sorted_actors[1].push_back(std::static_pointer_cast<state::Actor>(kings[1]));

	scouts[0][0] = std::shared_ptr<state::Scout>(new state::Scout(6, state::PLAYER1, 0, 300, 300, 37, 10,
		90, 0, 0, physics::Vector2D(4 * ELEMENT_SIZE, 4 * ELEMENT_SIZE), physics::Vector2D(0, 0),
		6, 0, 0));
	scouts[0][0]->AddPathPlanner(state::PathPlannerHelper(scouts[0][0]));
	sorted_actors[0].push_back(std::static_pointer_cast<state::Actor>(scouts[0][0]));

	scouts[1][0] = std::shared_ptr<state::Scout>(new state::Scout(7, state::PLAYER2, 0, 300, 300, 37, 10,
		90, 0, 0, physics::Vector2D(27 * ELEMENT_SIZE, 27 * ELEMENT_SIZE),
		physics::Vector2D(0, 0), 6, 0, 0));
	scouts[1][0]->AddPathPlanner(state::PathPlannerHelper(scouts[1][0]));
	sorted_actors[1].push_back(std::static_pointer_cast<state::Actor>(scouts[1][0]));

	std::vector<physics::Vector2D> tower_pos{ physics::Vector2D(11, 5), physics::Vector2D(22, 8),
		physics::Vector2D(7, 13), physics::Vector2D(20, 26), physics::Vector2D(24, 18), physics::Vector2D(9, 23) };

	state::act_id_t id_count = 7;
	for (int64_t i = 0; i < 2; i++) {
		for (int64_t j = 0; j < 3; j++) {
			state::PlayerId p = static_cast<state::PlayerId>(i);
			towers[i][j] = std::shared_ptr<state::Tower>(new state::Tower(++id_count, p, 80, 600,
				600, 0, 10, 0, 0, 0, tower_pos[i * 3 + j] * ELEMENT_SIZE,
				physics::Vector2D(0, 0), 5, 40, 5 * ELEMENT_SIZE, 100, 5 * ELEMENT_SIZE, 60, 300, 10));
			sorted_actors[i].push_back(
				std::static_pointer_cast<state::Actor>(towers[i][j]));
		}
	}

	for (int64_t i = 0; i < 2; i++) {
		for (int64_t j = 0; j < 20; j++) {
			int team_pos = i == 0 ? 1 : 29;
			state::PlayerId p = static_cast<state::PlayerId>(i);
			swordsmen[i][j] = std::shared_ptr<state::Swordsman>(
				new state::Swordsman(++id_count, p, 20, 200, 200, 20, 10, 45, 0, 0,
					physics::Vector2D(team_pos * ELEMENT_SIZE, team_pos * ELEMENT_SIZE),
					physics::Vector2D(0, 0), 2, 10, 30));
			swordsmen[i][j]->AddPathPlanner(state::PathPlannerHelper(swordsmen[i][j]));
			sorted_actors[i].push_back(
				std::static_pointer_cast<state::Actor>(swordsmen[i][j]));
		}
	}

	for (int64_t i = 0; i < 2; i++) {
		for (int64_t j = 0; j < 10; j++) {
			int team_pos = i == 0 ? 1 : 29;
			state::PlayerId p = static_cast<state::PlayerId>(i);
			magicians[i][j] = std::shared_ptr<state::Magician>(
				new state::Magician(++id_count, p, 50, 150, 150, 30, 10, 60, 0, 0,
					physics::Vector2D(team_pos * ELEMENT_SIZE, team_pos * ELEMENT_SIZE),
					physics::Vector2D(0, 0), 3, 25, 3 * ELEMENT_SIZE, 60, 100, 10));
			magicians[i][j]->AddPathPlanner(state::PathPlannerHelper(magicians[i][j]));
			sorted_actors[i].push_back(
				std::static_pointer_cast<state::Actor>(magicians[i][j]));
		}
	}

	state::State S(terrain, sorted_actors, kings, bases, flags, towers, scouts,
		magicians, swordsmen);

	return S;
}

/**
 * Debugging method to print the terrain
 *
 * @param[in]  terrain  The terrain
 */
void PrintTT(state::Terrain terrain)
{
	for (int i = 0; i < terrain.GetRows(); i++) {
		for (int j = 0; j < terrain.GetRows(); j++) {
			std::cout << terrain.OffsetToTerrainElement(physics::Vector2D(i, j))
						.GetTerrainType()
				 << ' ';
		}
		std::cout << std::endl;
	}
}

/**
 * Debugging method to print LOS for a given player
 *
 * @param[in]  terrain    The terrain
 * @param[in]  player_id  The player ID
 */
void PrintLos(state::Terrain terrain, state::PlayerId player_id)
{
	for (int i = 0; i < terrain.GetRows(); i++) {
		for (int j = 0; j < terrain.GetRows(); j++) {
			std::cout << terrain.OffsetToTerrainElement(physics::Vector2D(i, j))
						.GetLos(player_id)
				 << ' ';
		}
		std::cout << std::endl;
	}
}

/**
 * Method to load a terrain from a plaintext file
 *
 * Elements are stored in one long line, and the terrain is assumed to
 * be a square
 *
 * Each element is one of:
 * - 'P': Plain
 * - 'M': Mountain
 * - 'F': Forest
 *
 * @param[in]  file_name  The file name
 *
 * @return     The terrain
 */
state::Terrain LoadTerrain(std::string file_name)
{
	std::string line;
	std::ifstream file(file_name);
	std::getline(file, line);
	int rows = (int)sqrt(line.size());
	int total = line.size();
	std::vector<std::vector<state::TerrainElement> > grid;
	for (int i = 0; i < rows; i++) {
		std::vector<state::TerrainElement> temp;
		for (int j = 0; j < rows; j++) {
			state::TERRAIN_TYPE t;
			switch (line[i * rows + j]) {
			case 'F':
				t = state::FOREST;
				break;
			case 'M':
				t = state::MOUNTAIN;
				break;
			default:
				t = state::PLAIN;
			}
			temp.push_back(state::TerrainElement(
				t, physics::Vector2D(i * ELEMENT_SIZE, j * ELEMENT_SIZE), ELEMENT_SIZE));
		}
		grid.push_back(temp);
	}
	return state::Terrain(grid);
}

int main(int argc, char * argv[])
{
	bool is_headless;
	std::string exec_path(argv[0]);
	exec_path = exec_path.substr(0, exec_path.size() - 4);

	if (argv[1][0] == 'r') {
		is_headless = false;
	}
	else {
		is_headless = true;
	}

	int level_number = 2;

	std::string terrain_file_path(argv[3]);

	state::Terrain TT(ipc::LoadTerrain(terrain_file_path));

	auto state = MakeState(TT);

	auto S = std::shared_ptr<state::State>(new state::State(state));

	auto S1 = std::shared_ptr<state::State>(new state::State(state));
	state::PlayerStateHandler PSH1(S1.get(), state::PLAYER1);

	auto S2 = std::shared_ptr<state::State>(new state::State(state));
	state::PlayerStateHandler PSH2(S2.get(), state::PLAYER2);

	drivers::MainDriver driver(player::PlayerAi(std::shared_ptr<player::PlayerAiHelper>(new ai::AI(level_number))),
		player::PlayerAi(std::shared_ptr<player::PlayerAiHelper>(new ai1::Ai1())), S, S1, S2, 5 * 60 * 1000, is_headless);

	driver.Run();

	driver.Join();

	return 0;
}
