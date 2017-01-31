#include <bits/stdc++.h>

int main() {
  std::string line;
  std::getline(std::ifstream("grid.txt"), line);
  std::vector<std::vector<char> > grid(60, std::vector<char>(60));
  for (size_t i = 0; i < line.size(); i++) {
    grid[i/60][i%60] = line[i];
  }
  for (auto line : grid) {
    for (auto c : line)
      std::cout << c << ' ';
    std::cout << std::endl;
  }
}
