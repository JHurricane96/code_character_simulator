#include "path_planner/path_planner.h"

namespace state {

PathPlanner::PathPlanner(int64_t map_size)
	: graph(map_size),
	  next_formation_id(std::vector<int64_t>({1})),
      formations(std::vector<std::vector<Formation> >(2)) {}

void PathPlanner::MakeFormation(
	PlayerId player_id,
	std::vector<std::shared_ptr<Actor> > &units,
	Terrain &terrain,
	std::shared_ptr<FormationMaker> formation_maker,
	physics::Vector2D destination,
	std::vector<int64_t> terrain_weights
) {
	std::vector<physics::Vector2D> destinations;
	PlanPath(
		units[0]->GetPosition(),
		destination,
		terrain,
		destinations,
		terrain_weights
	);
	formations[player_id].push_back(Formation(
		player_id,
		next_formation_id[player_id]++,
		units,
		formation_maker,
		destinations
	));
}

int64_t PathPlanner::PlanPath(
	physics::Vector2D start_point,
	physics::Vector2D destination,
	Terrain &terrain,
	std::vector<physics::Vector2D> &next_points,
	std::vector<int64_t> terrain_weights
) {
	return graph.FindPath(
		start_point,
		destination,
		terrain,
		next_points,
		terrain_weights
	);
}

void PathPlanner::Update(
	std::vector<std::vector<std::shared_ptr<Actor> > > &sorted_units
) {
	for (
		int64_t cur_player_id = 0;
		cur_player_id < formations.size();
		++cur_player_id
	) {
		std::vector<Formation> new_formations;
		for (auto formation : formations[cur_player_id]) {
			formation.Update(
				sorted_units[cur_player_id],
				sorted_units[(cur_player_id + 1) % (LAST_PLAYER + 1)]
			);
			if (!formation.IsFinished()) {
				new_formations.push_back(formation);
			}
		}
		formations[cur_player_id] = new_formations;
	}
}

void PathPlanner::MergeWithBuffer(
	const PathPlanner& path_planner,
	PlayerId player_id,
	std::vector<std::shared_ptr<Actor> > actors
) {
	formations[player_id].clear();
	for (auto formation : path_planner.formations[player_id]) {
		formations[player_id].push_back(formation);
		formations[player_id].back().MergeWithBuffer(formation, actors);
	}
	next_formation_id[player_id] =
		path_planner.next_formation_id[player_id];
}

}
