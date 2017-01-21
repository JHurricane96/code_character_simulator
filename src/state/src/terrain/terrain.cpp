#include <terrain/terrain.h>
#include <cmath>
#include <queue>

namespace state {

LosListEntry::LosListEntry(physics::Vector2D offset, float score)
	: offset(offset), score(score) {}

bool LosListEntry::operator<(const LosListEntry& rhs) {
	return score < rhs.score;
}

Terrain::Terrain(std::vector<std::vector<TerrainElement> > grid)
	: row_size(grid.size()), grid(grid) {
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

void Terrain::UpdateLos(
	physics::Vector2D offset,
	int64_t radius,
	LOS_TYPE los,
	PlayerId pid
) {
	std::vector<std::vector<bool> >visited(
		row_size,
		std::vector<bool>(row_size, false)
	);
	std::queue<LosListEntry> q;
	q.push(LosListEntry(offset, radius - 1));
	visited[offset.y][offset.y] = true;
	auto neighbours = adjacent_neighbours;
	while (!q.empty()) {
		auto top = q.front();
		q.pop();
		auto pos = top.offset;
		grid[pos.x][pos.y].SetLos(los, pid);
		auto rad = top.score;
		for (auto i : adjacent_neighbours) {
			auto v = pos + i;
			if (v.x >= 0 && v.x < row_size && v.y >= 0 && v.y < row_size)
				if (!visited[v.x][v.y] && rad > 0) {
					visited[v.x][v.y] = true;
					auto multiplier =
						Multiplier[grid[pos.x][pos.y].GetTerrainType()]
								  [grid[v.x][v.y].GetTerrainType()];
					q.push(LosListEntry(v, rad - (1 / multiplier)));
				}
		}
	}
}

void Terrain::Update(
	std::vector<std::vector<std::shared_ptr<Actor> > > actors
) {
	for(int64_t i = 0; i < row_size; i++)
		for(int64_t j = 0; j < row_size; j++)
			for(int64_t pid = 0; pid <= LAST_PLAYER; pid++)
				if (grid[i][j].GetLos(static_cast<PlayerId>(pid)) == DIRECT_LOS) {
					grid[i][j].SetLos(EXPLORED, static_cast<PlayerId>(pid));
				}

	for (int64_t i = 0; i <= LAST_PLAYER; i++) {
		for (auto &actor: actors[i]) {
			if (actor) {
				auto pos = actor->GetPosition();
				int64_t size = grid[0][0].GetSize();
				auto offset = physics::Vector2D((int)pos.x/size, (int)pos.y/size);
				UpdateLos(
					offset,
					actor->GetLosRadius(),
					DIRECT_LOS,
					static_cast<PlayerId>(i)
				);
			}
		}
	}
}

void Terrain::MergeWithMain(const Terrain& terrain) {
	for (int64_t i = 0; i < row_size; ++i) {
		for (int64_t j = 0; j < row_size; ++j) {
			grid[i][j].MergeWithMain(terrain.grid[i][j]);
		}
	}
}

}
