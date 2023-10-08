#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

const int INF = 100000;
const std::string path = "./input/input.txt";
typedef std::vector<std::vector<int>> matrix;

// data storage structure
struct Data {
  int num_node, num_state;
  matrix dis_matrix;
  matrix dp_table;

  // 狀態的數量為 2^(num_node)
  Data(int num_node) : num_node(num_node), num_state(1 << (num_node - 1)) {
    dis_matrix = matrix(num_node, std::vector<int>(num_node, INF));
    for (int i = 0; i < num_node; ++i) dis_matrix[i][i] = 0;
    dp_table = matrix(num_state, std::vector<int>(num_node, INF));
  }
};

// Input functions
auto read_input() {
  struct Input_Data {
    int num_node;
    std::vector<int> input_stream;

    Input_Data(int num_node, std::vector<int> input_stream)
        : num_node(num_node), input_stream(input_stream) {}
  };

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

  Input_Data input_data(num_node, vector_tmp);

  return input_data;
}

auto build_dis_matrix(const std::vector<int>& input_stream, int num_node)
    -> matrix {
  matrix dis_matrix(num_node, std::vector<int>(num_node, INF));
  for (int i = 0; i < num_node; ++i) dis_matrix[i][i] = 0;

  for (int i = 0; i < input_stream.size(); i += 4) {
    dis_matrix[input_stream[i + 2] - 1][input_stream[i + 3] - 1] =
        input_stream[i + 1];
    dis_matrix[input_stream[i + 3] - 1][input_stream[i + 2] - 1] =
        input_stream[i + 1];
  }

  return dis_matrix;
}

auto calculate_min_distance(int state, int node, int num_node, int num_state,
                            const matrix& dp_table, const matrix& dis_matrix)
    -> int {
  // 確認 node 是否在集合內
  bool node_in_state = (state & (1 << (node - 1)));
  int min_dis = dp_table[state][node];

  // 空集合：從起點到 node 的距離（起點到起點為 0）
  if (state == 0)
    return dis_matrix[0][node];
  else if (node == 0 and state != num_state - 1)
    return INF;
  else if (node_in_state)
    return INF;

  // 若 node 不在集合內，從集合內找出 min(dp (i, {state - i}) + dis (i, node))
  for (int i = 1; i < num_node; ++i) {
    // 若節點 i 不在集合內，跳過
    bool i_in_state = (state & (1 << (i - 1)));
    if (not i_in_state) continue;

    auto state_except_i = state ^ (1 << (i - 1));
    auto a = dp_table[state_except_i][i];
    auto b = dis_matrix[i][node];

    // 若 dp (node, {state}) > dp (i, {state - i}) + dis (i, node)
    if (min_dis > a + b)
      // dp (node, {state}) = dp (i, {state - i}) + dis (i, node)
      min_dis = dp_table[state_except_i][i] + dis_matrix[i][node];
  }

  return min_dis;
}

// Constuct dp table
auto build_dp_table(int num_node, int num_state, const matrix& dis_matrix) {
  matrix dp_table(num_state, std::vector<int>(num_node, INF));

  // 從 0 開始遍歷所有路徑狀態
  for (int i = 0; i < num_state; ++i) {
    // 對每一條路徑狀態，尋找前往下一個節點的所需的最短路徑
    for (int j = 0; j < num_node; ++j) {
      dp_table[i][j] = calculate_min_distance(i, j, num_node, num_state,
                                              dp_table, dis_matrix);
    }
  }

  return dp_table;
}

// TODO: get the path of the shortest path

// TODO: output the result to file

int main(int, char**) {
  auto input_data = read_input();
  auto dis_matrix =
      build_dis_matrix(input_data.input_stream, input_data.num_node);
  auto num_state = 1 << (input_data.num_node - 1);

  auto dp_table = build_dp_table(input_data.num_node, num_state, dis_matrix);

  std::cout << (*(dp_table.end() - 1))[0] << std::endl;

  return 0;
}
