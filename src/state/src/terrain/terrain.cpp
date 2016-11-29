#include <terrain/terrain.h>

namespace state {

Terrain::Terrain(std::vector<std::vector<TerrainElement> > grid) :
	grid(grid), row_size(grid.size()) {}

Terrain::Terrain(int64_t nrows) {
	row_size = nrows;
	grid.resize(nrows);
	for (auto row:grid)
		row.resize(nrows);
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

}
