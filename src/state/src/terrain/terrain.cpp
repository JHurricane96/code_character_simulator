#include <terrain/terrain.h>

namespace state {

Terrain::Terrain(std::vector<std::vector<TerrainElement> > grid) :
	grid(grid),
	row_size(grid.size()) {
		adj_neighbours = std::vector<physics::Vector2D>({Vector2D(0,1), Vector2D(1,0), Vector2D(0,-1), Vector2D(-1,0)});
		dia_neighbours = std::vector<physics::Vector2D>({Vector2D(1,1), Vector2D(1,-1), Vector2D(-1,-1), Vector2D(-1,1)});
	}

Terrain::Terrain(int64_t nrows) {
	row_size = nrows;
	grid.resize(nrows);
	for (auto row:grid)
		row.resize(nrows);
	adj_neighbours = std::vector<physics::Vector2D>({Vector2D(0,1), Vector2D(1,0), Vector2D(0,-1), Vector2D(-1,0)});
	dia_neighbours = std::vector<physics::Vector2D>({Vector2D(1,1), Vector2D(1,-1), Vector2D(-1,-1), Vector2D(-1,1)});

}

TerrainElement Terrain::CoordinateToTerrainElement(physics::Vector2D position) {
	int64_t size = grid[0][0].getSize();
	return grid[(int)position.x / size][(int)position.y / size];
}

TerrainElement Terrain::OffsetToTerrainElement(physics::Vector2D offset) {
	return grid[offset.x][offset.y];
}

physics::Vector2D Terrain::GetSize() {
	TerrainElement last =  grid[row_size-1][row_size-1];
	return last.GetPosition() + last.GetSize();
}

std::vector<physics::Vector2D> Terrain::GetAdjacentNeighbours(physics::Vector2D offset){
	std::vector<physics::Vector2D> neighbours(4);
	for(int64_t i = 0; i < 4; i++)
		neighbours[i] = offset + adj_neighbours[i];
	return neighbours;
}

std::vector<physics::Vector2D> Terrain::GetDiagonalNeighbours(physics::Vector2D offset){
	std::vector<physics::Vector2D> neighbours(4);
	for(int64_t i = 0; i < 4; i++)
		neighbours[i] = offset + dia_neighbours[i];
	return neighbours;
}

std::vector<physics::Vector2D> Terrain::GetAllNeighbours(physics::Vector2D offset){
	std::vector<physics::Vector2D> neighbours = GetAdjacentNeighbours(offset);
	std::vector<physics::Vector2D> dia = GetDiagonaltNeighbours(offset);
	neighbours.insert(neighbours.end(), dia.begin(), dia.end());
	return neighbours;
}

}
