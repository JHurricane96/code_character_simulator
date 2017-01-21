#include <cmath>
#include <algorithm>
#include "path_planner/graph.h"

namespace state {

OpenListEntry::OpenListEntry(
	physics::Vector2D node,
	int64_t weight,
	int64_t time_added) :
	node(node), weight(weight), time_added(time_added) {}

bool OpenListEntry::operator<(const OpenListEntry& rhs) const {
	return weight > rhs.weight;
}

Graph::Graph(int64_t map_size) : map_size(map_size) {
	in_open_list = init_matrix(false, map_size);
	in_closed_list = init_matrix(false, map_size);
	g = init_matrix((float)-1.0, map_size);
	parents = init_matrix(physics::Vector2D(-1, -1), map_size);
	last_added = init_matrix((int64_t)-1, map_size);
}

void Graph::InitGraph(
	Terrain& terrain,
	std::vector<int64_t> terrain_weights
) {
	for (int64_t i = 0; i < map_size; ++i) {
		for (int64_t j = 0; j < map_size; ++j) {
			in_open_list[i][j] = in_closed_list[i][j] = false;
			g[i][j] = -1.0;
			last_added[i][j] = -1;
			parents[i][j] = physics::Vector2D(-1, -1);
		}
	}
	cur_time = 0;
	open_list = std::priority_queue<OpenListEntry>();
	this->terrain = &terrain;
	this->terrain_weights = terrain_weights;
}

float Graph::FindNodeDistance(
	physics::Vector2D node_a,
	physics::Vector2D node_b
) {
	return (node_a.distance(node_b) *
		*std::min_element(
			terrain_weights.begin(),
			terrain_weights.end()
		));
}

void Graph::SetNodeCost(
	physics::Vector2D node,
	physics::Vector2D adj_node
) {
	float new_weight = g[node.x][node.y] +
		terrain_weights[
			terrain->OffsetToTerrainElement(adj_node).GetTerrainType()
		];
	if (new_weight < g[adj_node.x][adj_node.y]) {
		parents[adj_node.x][adj_node.y] = node;
		g[adj_node.x][adj_node.y] = new_weight;
	}
}

void Graph::UpdateNode(
	physics::Vector2D node,
	physics::Vector2D adj_node,
	physics::Vector2D destination
){
	float g_old = g[adj_node.x][adj_node.y];
	SetNodeCost(node, adj_node);
	if (g[adj_node.x][adj_node.y] < g_old) {
		OpenListEntry new_entry(
			adj_node,
			g[adj_node.x][adj_node.y] +
				FindNodeDistance(adj_node, destination),
			cur_time
		);
		last_added[adj_node.x][adj_node.y] = cur_time++;
		open_list.push(new_entry);
	}
}

float Graph::FindPath(
	physics::Vector2D start_point,
	physics::Vector2D destination,
	Terrain &terrain,
	std::vector<physics::Vector2D> &next_points,
	std::vector<int64_t> terrain_weights
) {
	auto terrain_elt_size = terrain
		.OffsetToTerrainElement(physics::Vector2D(0, 0))
		.GetSize();
	start_point = start_point / terrain_elt_size;
	start_point.x = floor(start_point.x);
	start_point.y = floor(start_point.y);
	next_points.push_back(destination);
	destination = destination / terrain_elt_size;
	InitGraph(terrain, terrain_weights);
	OpenListEntry cur_node(start_point, 0, cur_time);
	open_list.push(cur_node);
	g[start_point.x][start_point.y] = 0;
	in_open_list[start_point.x][start_point.y] = true;
	parents[start_point.x][start_point.y] = start_point;
	last_added[start_point.x][start_point.y] = cur_time++;

	while (open_list.empty() == false) {
		cur_node = open_list.top();
		open_list.pop();
		in_closed_list[cur_node.node.x][cur_node.node.y] = true;

		if (cur_node.node == destination) {
			break;
		}

		if (cur_node.time_added !=
			last_added[cur_node.node.x][cur_node.node.y]) {
			continue;
		}

		std::vector<physics::Vector2D> neighbours =
			terrain.GetAllNeighbours(cur_node.node);

		for (auto neighbour : neighbours) {
			if (in_closed_list[neighbour.x][neighbour.y] == false) {
				if (in_open_list[neighbour.x][neighbour.y] == false) {
					g[neighbour.x][neighbour.y] = INT64_MAX;
					in_open_list[neighbour.x][neighbour.y] = true;
				}
				UpdateNode(cur_node.node, neighbour, destination);
			}
		}
	}

	physics::Vector2D seek_node = destination;
	while (!(seek_node == start_point)) {
		next_points.push_back(
			terrain.OffsetToTerrainElement(seek_node).GetPosition()
		);
		seek_node = parents[seek_node.x][seek_node.y];
	}


	return g[cur_node.node.x][cur_node.node.y];
}

}
