/**
 * @file graph.h
 * Contains class that does path finding
 * 
 * This class does the actual path finding
 */

#ifndef STATE_PATH_PLANNER_GRAPH_H
#define STATE_PATH_PLANNER_GRAPH_H

#include <vector>
#include <memory>
#include <cstdint>
#include <queue>
#include "vector2d.h"
#include "utilities.h"
#include "terrain/terrain.h"
#include "state_export.h"

namespace state {

/**
 * 2D matrix
 */
template<typename T>
using matrix = std::vector<std::vector<T> >;

/**
 * Initializes matrix with elements of the given value and returns it
 *
 * @param[in]  init_value  The initialize value
 * @param[in]  no_rows     Number of rows
 * @param[in]  no_cols     Number of columns
 *
 * @tparam     T           Data type matrix holds
 *
 * @return     The initialized 2D matrix
 */
template<typename T>
matrix<T> init_matrix(T init_value, int64_t no_rows, int64_t no_cols) {
	std::vector<T> row(no_cols, init_value);
	matrix<T> a(no_rows, row);
	return a;
}

/**
 * Initializes matrix with elements of the given value and returns it
 *
 * @param[in]  init_value  The initialize value
 * @param[in]  size        The size of the matrix
 *
 * @tparam     T           Data type matrix holds
 *
 * @return     The initialized 2D matrix
 */
template<typename T>
matrix<T> init_matrix(T init_value, int64_t size) {
	return init_matrix(init_value, size, size);
}

/**
 * An entry in the priority queue while a graph search is ongoing
 */
struct OpenListEntry {
	/**
	 * Position of the node
	 */
	physics::Vector2D node;
	/**
	 * Weight of the node
	 * 
	 * weight = g + h
	 * g = distance from source
	 * h = estimated distance to destination
	 */
	float weight;
	/**
	 * The time that this entry was added to the queue
	 * 
	 * Used to discard duplicate entries
	 */
	int64_t time_added;
	/**
	 * Constructor for OpenListEntry
	 *
	 * @param[in]  node        Position of the node
	 * @param[in]  weight      Weight of the node
	 * @param[in]  time_added  Time when entry was made
	 */
	OpenListEntry(
		physics::Vector2D node,
		int64_t weight,
		int64_t time_added
	);
	/**
	 * Less than operator
	 *
	 * @param[in]  rhs   Entry on the right hand side
	 *
	 * @return     true if this entry is less, false otherwise
	 */
	bool operator<(const OpenListEntry& rhs) const;
};

/**
 * Class that does the path finding
 * 
 * Returns a path and its weight given the necessary details
 */
class STATE_EXPORT Graph {
private:
	/**
	 * Size of the map to search
	 */
	int64_t map_size;
	/**
	 * Bitmap of nodes in the open list
	 */
	matrix<bool> in_open_list;
	/**
	 * Bitmap of nodes in the closed list
	 */
	matrix<bool> in_closed_list;
	/**
	 * Distances of nodes from the source
	 */
	matrix<float> g;
	/**
	 * Parents of nodes
	 */
	matrix<physics::Vector2D> parents;
	/**
	 * Map of times when nodes were last added to the open list
	 */
	matrix<int64_t> last_added;
	/**
	 * The current time
	 * 
	 * Incremented each time when node is added to the open list
	 */
	int64_t cur_time;
	/**
	 * The open list
	 * 
	 * The next nodes to visit in the search are stored here
	 */
	std::priority_queue<OpenListEntry> open_list;
	/**
	 * Pointer to the terrain that the graph search uses
	 */
	Terrain* terrain;
	/**
	 * Weights determining terrain preference while path finding
	 */
	std::vector<int64_t> terrain_weights;
	/**
	 * Helper method for path finding
	 * 
	 * Finds estimated distance between two nodes
	 *
	 * @param[in]  node_a  A node
	 * @param[in]  node_b  Another node
	 */
	float FindNodeDistance(
		physics::Vector2D node_a,
		physics::Vector2D node_b
	);
	/**
	 * Helper method for path finding
	 *
	 * Sets the node cost (g value)
	 *
	 * @param[in]  node      The node from which this node was 
	 *                       discovered
	 * @param[in]  adj_node  The node to update
	 */
	void SetNodeCost(physics::Vector2D node, physics::Vector2D adj_node);
	/**
	 * Helper method for path finding
	 * 
	 * Updates a node when its discovered as a neighbour
	 * Sets g value and parent
	 *
	 * @param[in]  node         The node from which this node was 
	 *                          discovered
	 * @param[in]  adj_node     The node to update
	 * @param[in]  destination  The destination
	 */
	void UpdateNode(
		physics::Vector2D node,
		physics::Vector2D adj_node,
		physics::Vector2D destination
	);
public:
	/**
	 * Constructor for Graph
	 *
	 * @param[in]  map_size  Size of the map to search
	 */
	Graph(int64_t map_size);
	/**
	 * Initializes the Graph
	 * 
	 * Called right before this is used to find a path
	 *
	 * @param      terrain          The terrain
	 * @param[in]  terrain_weights  Weights determining terrain
	 *                              preference while path finding
	 */
	void InitGraph(
		Terrain& terrain,
		std::vector<int64_t> terrain_weights
	);
	/**
	 * Finds a path and its weight from a node to another
	 *
	 * @param[in]  start_point      The start point
	 * @param[in]  destination      The destination
	 * @param      terrain          The terrain the graph search uses
	 * @param      next_points      The path is stored here
	 * @param[in]  terrain_weights  Weights determining terrain
	 *                              preference while path finding
	 *
	 * @return     Weight of the path
	 */
	float FindPath(
		physics::Vector2D start_point,
		physics::Vector2D destination,
		Terrain &terrain,
		std::vector<physics::Vector2D> &next_points,
		std::vector<int64_t> terrain_weights
	);
};

}

#endif
