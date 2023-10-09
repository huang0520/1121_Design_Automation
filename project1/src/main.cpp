#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

const int INF = 100000;
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
auto read_input(std::string input_path) {
  struct Input_Data {
    int num_node;
    std::vector<int> input_stream;

    Input_Data(int num_node, std::vector<int> input_stream)
        : num_node(num_node), input_stream(input_stream) {}
  };

  std::ifstream input(input_path);

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

auto write_output(std::string output_path, std::vector<int> shortest_path,
                  int shortest_dis, double execution_time) -> void {
  std::ofstream output(output_path, std::ios::out);

  output << "Total distance: " << shortest_dis << "\n";
  output << "Oredering of cities:\n  ";
  for (auto it = shortest_path.rbegin(); it != shortest_path.rend(); ++it) {
    output << "v" << *it + 1 << " ";
  }
  output << "\n";
  output << "Execution time: " << std::fixed << execution_time
         << std::setprecision(5) << " seconds\n";

  output.close();
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
  else if (node_in_state or (node == 0 and state != (num_state - 1)))
    return INF;

  // 若 node 不在集合內，從集合內找出 min(dp (i, {state - i}) + dis (i, node))
  for (int i = 1; i < num_node; ++i) {
    // 若節點 i 不在集合內，跳過
    bool i_in_state = (state & (1 << (i - 1)));
    if (not i_in_state) continue;

    auto state_except_i = state ^ (1 << (i - 1));

    // 若 dp (node, {state}) > dp (i, {state - i}) + dis (i, node)
    if (min_dis > dp_table[state_except_i][i] + dis_matrix[i][node])
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

auto get_shortest_path(int num_node, int num_state, const matrix& dis_matrix,
                       const matrix& dp_table) -> std::vector<int> {
  // 從 0 回頭
  std::vector<int> path(1, 0);
  int state = num_state - 1;
  int node = 0;

  while (state != 0) {
    int min_dis = INF;
    int pre_node;

    for (int i = 1; i < num_node; ++i) {
      // 若節點 i 不在集合內，跳過
      bool i_in_state = (state & (1 << (i - 1)));
      if (not i_in_state) continue;

      // 若節點 i 在集合內，找出 d(i, {state - i})
      int state_except_i = state ^ (1 << (i - 1));

      // 最小的 d(i, {state - i}) 即為前一個節點
      if (min_dis > dp_table[state_except_i][i] + dis_matrix[i][node]) {
        min_dis = dp_table[state_except_i][i] + dis_matrix[i][node];
        pre_node = i;
      }
    }

    auto state_without_pre_node = state ^ (1 << (pre_node - 1));
    state = state_without_pre_node;

    node = pre_node;

    path.emplace_back(node);
    // 最後到 state = 0 時，將起點加入 path
    if (state == 0) path.emplace_back(0);
  }

  return path;
}

// dp(0, {1, 2, 3 ..... n}) 即為最短距離
auto get_shortest_dis(const matrix& dp_table) -> int {
  return dp_table[dp_table.size() - 1][0];
}

int main(int argc, char** argv) {
  std::string input_path = argv[1];
  std::string output_path = argv[2];

  auto start_time = std::chrono::high_resolution_clock::now();

  auto input_data = read_input(input_path);
  auto dis_matrix =
      build_dis_matrix(input_data.input_stream, input_data.num_node);
  auto num_state = 1 << (input_data.num_node - 1);

  auto dp_table = build_dp_table(input_data.num_node, num_state, dis_matrix);

  auto shortest_path =
      get_shortest_path(input_data.num_node, num_state, dis_matrix, dp_table);
  auto shortest_dis = get_shortest_dis(dp_table);

  auto end_time = std::chrono::high_resolution_clock::now();
  double execution_time =
      std::chrono::duration<double, std::milli>(end_time - start_time).count() /
      1000.;

  write_output(output_path, shortest_path, shortest_dis, execution_time);
  return 0;
}
