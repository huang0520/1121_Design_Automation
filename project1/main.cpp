#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Print graph
auto print_graph(std::vector<std::vector<int>>& graph) -> void {
  for (auto& n : graph) {
    for (auto i : n) {
      std::cout << i << " ";
    }
    std::cout << "\n";
  }
}

// IO functions
auto read(std::string path) -> std::vector<std::vector<int>> {
  std::ifstream input(path);

  int num_node;
  std::vector<int> vector_tmp;

  while (!input.eof()) {
    char tmp;
    std::vector<int> input_num_tmp(4, 0);

    // Read only the number of input and put it into a vector
    for (int i = 0; i < 4; ++i) {
      if (i == 1) {
        input >> input_num_tmp[i];
      } else {
        input >> tmp >> input_num_tmp[i];
      }
      if (input.eof()) break;

      vector_tmp.emplace_back(input_num_tmp[i]);
    }

    // Find the number of nodes
    num_node = std::max(num_node, std::max(input_num_tmp[2], input_num_tmp[3]));
  };
  input.close();

  // Construct the graph
  std::vector<std::vector<int>> graph(num_node, std::vector<int>(num_node, 0));

  for (int i = 0; i < vector_tmp.size(); i += 4) {
    graph[vector_tmp[i + 2] - 1][vector_tmp[i + 3] - 1] = vector_tmp[i + 1];
    graph[vector_tmp[i + 3] - 1][vector_tmp[i + 2] - 1] = vector_tmp[i + 1];
  }

  return graph;
}

int main(int, char**) {
  std::string path = "./input.txt";

  auto graph = read(path);

  print_graph(graph);

  return 0;
}
