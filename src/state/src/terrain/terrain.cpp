#include <terrain/terrain.h>

namespace state {

Terrain::Terrain(std::vector<std::vector<TerrainElement> > grid)
	: grid(grid), row_size(grid.size()) {
	adjacent_neighbours = std::vector<physics::Vector2D>({
		physics::Vector2D(0,1),
		physics::Vector2D(1,0),
		physics::Vector2D(0,-1),
		physics::Vector2D(-1,0)
	});
	diagonal_neighbours = std::vector<physics::Vector2D>({
		physics::Vector2D(1,1),
		physics::Vector2D(1,-1),
		physics::Vector2D(-1,-1),
		physics::Vector2D(-1,1)
	});
}

Terrain::Terrain(int64_t nrows) {
	row_size = nrows;
	grid.resize(nrows);
	for (auto row:grid)
		row.resize(nrows);
	adjacent_neighbours = std::vector<physics::Vector2D>({
		physics::Vector2D(0,1),
		physics::Vector2D(1,0),
		physics::Vector2D(0,-1),
		physics::Vector2D(-1,0)
	});
	diagonal_neighbours = std::vector<physics::Vector2D>({
		physics::Vector2D(1,1),
		physics::Vector2D(1,-1),
		physics::Vector2D(-1,-1),
		physics::Vector2D(-1,1)
	});
}

TerrainElement Terrain::CoordinateToTerrainElement(physics::Vector2D position) {
	int64_t size = grid[0][0].GetSize();
	return grid[(int)position.x / size][(int)position.y / size];
}

TerrainElement Terrain::OffsetToTerrainElement(physics::Vector2D offset) {
	return grid[offset.x][offset.y];
}

physics::Vector2D Terrain::GetSize() {
	TerrainElement last =  grid[row_size-1][row_size-1];
	return last.GetPosition() + last.GetSize();
}

int64_t Terrain::GetRows() {
	return row_size;
}

std::vector<physics::Vector2D> Terrain::GetAdjacentNeighbours(physics::Vector2D offset, int64_t width) {
	std::vector<physics::Vector2D> neighbours;
	double width_offset = (double)width / grid[0][0].GetSize();
	for(int64_t i = 0; i < 4; i++) {
		double x_offset_tot = offset.x + adjacent_neighbours[i].x;
		double y_offset_tot = offset.y + adjacent_neighbours[i].y;
		if ( x_offset_tot >= width_offset && x_offset_tot < row_size - width_offset &&
			 y_offset_tot >= width_offset && y_offset_tot < row_size - width_offset )
			neighbours.push_back(offset + adjacent_neighbours[i]);
	}
	return neighbours;
}

std::vector<physics::Vector2D> Terrain::GetDiagonalNeighbours(physics::Vector2D offset, int64_t width) {
	std::vector<physics::Vector2D> neighbours;
	double width_offset = (double)width / grid[0][0].GetSize();
	for(int64_t i = 0; i < 4; i++) {
		double x_offset_tot = offset.x + diagonal_neighbours[i].x;
		double y_offset_tot = offset.y + diagonal_neighbours[i].y;
		if ( x_offset_tot >= width_offset && x_offset_tot < row_size - width_offset &&
			 y_offset_tot >= width_offset && y_offset_tot < row_size - width_offset )
			neighbours.push_back(offset + diagonal_neighbours[i]);
	}
	return neighbours;
}

std::vector<physics::Vector2D> Terrain::GetAllNeighbours(physics::Vector2D offset, int64_t width) {
	std::vector<physics::Vector2D> neighbours = GetAdjacentNeighbours(offset, width);
	std::vector<physics::Vector2D> diagonals = GetDiagonalNeighbours(offset, width);
	neighbours.insert(neighbours.end(), diagonals.begin(), diagonals.end());
	return neighbours;
}

void Terrain::Update(
	std::vector<std::vector<std::shared_ptr<Actor> > > actors
) {
	for(int64_t i = 0; i < row_size; i++)
		for(int64_t j = 0; j < row_size; j++)
			for(int64_t pid = 0; pid <= LAST_PLAYER; pid++)
				if (grid[i][j].GetLos(pid) == DIRECT_LOS) {
					grid[i][j].SetLos(EXPLORED, pid);
					grid[i][j].SetUnits((PlayerId)pid, list_act_id_t());
				}

	for (int64_t i = 0; i <= LAST_PLAYER; i++) {
		for (auto &actor: actors[i]) {
			if (actor) {
				auto pos = actor->GetPosition();
				int64_t size = grid[0][0].GetSize();
				grid[(int)pos.x / size][(int)pos.y / size]
					.AddUnit((PlayerId)i, actor->GetId());
			}
		}
	}
}

}
