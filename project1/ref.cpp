#include <cfloat>   //用於處理浮點數的最大值
#include <chrono>   //用於測試程式執行時間
#include <cstdlib>  //字符串轉換為浮點數
#include <fstream>
#include <iostream>
#include <map>  //C++的標準映射（map）
#include <string>
#include <vector>
using namespace std;
#define result pair<double, vector<int>>
#define InputFile "input.txt"  // input file1
// #define InputFile "input2.txt" //input file2
//  讀取檔案並初始化城市距離數據
void ReadFile(vector<vector<double>> &cityDistance, double &overflowAmount,
              int &cityNum, bool &EdgeNotBig, double &maxDistance) {
  int maxCityNum = 0;
  ifstream ifile(InputFile);

  if (!ifile) {
    cout << "Could not open file" << endl;
    exit(1);
  }
  string inputTemp;
  while (ifile >> inputTemp) {
    vector<double> vecTemp;
    double inputTemp_double;

    for (int i = 0; i < 4; i++) {
      if (i == 1) {
        inputTemp_double = stod(inputTemp);
        vecTemp.push_back(inputTemp_double);
        maxDistance = max(maxDistance, inputTemp_double);
      } else {
        inputTemp.erase(0, 1);
        inputTemp_double = stod(inputTemp);
        vecTemp.push_back(inputTemp_double);
      }
      if (i < 3) {
        ifile >> inputTemp;
      }
    }
    cityDistance.push_back(vecTemp);
  }
  ifile.close();
  for (const vector<double> &record : cityDistance) {
    maxCityNum = max(maxCityNum, static_cast<int>(record[2]));
    maxCityNum = max(maxCityNum, static_cast<int>(record[3]));
  }
  cityNum = maxCityNum;
  EdgeNotBig = (maxDistance < (DBL_MAX / cityNum));
  if (!EdgeNotBig) {
    maxDistance = 1.0;
    for (vector<double> &record : cityDistance) {
      record[1] /= maxDistance;
    }
  }
}
// 建構城市距離矩陣
void CityDistanceMatrixConstructor(const vector<vector<double>> &cityDistance,
                                   int cityNum,
                                   vector<vector<double>> &cityDistanceMatrix) {
  cityDistanceMatrix.resize(cityNum,
                            vector<double>(cityNum, DBL_MAX / cityNum));

  for (const vector<double> &distance : cityDistance) {
    int begin_vertex = static_cast<int>(distance[2]) - 1;
    int end_vertex = static_cast<int>(distance[3]) - 1;
    cityDistanceMatrix[begin_vertex][end_vertex] = distance[1];
    cityDistanceMatrix[end_vertex][begin_vertex] = distance[1];
  }
}
// 動態規劃計算最短路徑
result dp(int i, int s, vector<vector<double>> &cityDistanceMatrix,
          map<pair<int, int>, result> &mp) {
  if (mp.find(make_pair(i, s)) != mp.end()) {
    return mp[make_pair(i, s)];
  }
  if ((1 << i) == s) {
    mp[make_pair(i, s)] = result(cityDistanceMatrix[0][i], {i});
    return result(cityDistanceMatrix[0][i], {i});
  }
  int t = s - (1 << i);
  double curmin = DBL_MAX;
  result r;
  for (int j = 1; j < cityDistanceMatrix.size(); j++) {
    if ((1 << j) & t) {
      result temp = dp(j, t, cityDistanceMatrix, mp);
      if (curmin > temp.first + cityDistanceMatrix[j][i]) {
        curmin = temp.first + cityDistanceMatrix[j][i];
        r = temp;
      }
    }
  }
  r.second.push_back(i);
  mp[make_pair(i, s)] = result(curmin, r.second);
  return result(curmin, r.second);
}
int main() {
  chrono::steady_clock::time_point tBegin = chrono::steady_clock::now();
  int cityNum = 0;
  double overflowAmount = 0, maxDistance = 0;
  bool EdgeNotBig = 1;
  vector<vector<double>> cityDistance;
  vector<vector<double>> cityDistanceMatrix;
  ofstream ofile;
  ReadFile(cityDistance, overflowAmount, cityNum, EdgeNotBig, maxDistance);
  CityDistanceMatrixConstructor(cityDistance, cityNum, cityDistanceMatrix);
  map<pair<int, int>, result> mp;  // Add this map
  result r = dp(0, ((1 << cityDistanceMatrix.size()) - 1), cityDistanceMatrix,
                mp);  // Pass the map
  chrono::steady_clock::time_point tEnd = chrono::steady_clock::now();
  int timeMicro =
      chrono::duration_cast<chrono::microseconds>(tEnd - tBegin).count();
  float executionTime = timeMicro / 1000000.0;
  if (EdgeNotBig) {
    cout << " Best distance is " << r.first << endl;
  } else {
    cout << " Best distance is " << (r.first) * maxDistance << endl;
  }
  cout << " Best path is ";
  for (int i = r.second.size() - 1; i >= 0; i--) {
    if (i == 0) {
      cout << "v" + to_string(r.second[i] + 1) << " → "
           << "v1";
    } else {
      cout << "v" + to_string(r.second[i] + 1) << " → ";
    }
  }
  cout << endl << " Execution time is " << executionTime << " seconds" << endl;
  ofile.open("output.txt", ios::out | ios::trunc);
  if (EdgeNotBig) {
    ofile << " Best distance is " << r.first << endl;
  } else {
    ofile << " Best distance is " << (r.first) * maxDistance << endl;
  }
  ofile << " Best path is ";
  for (int i = r.second.size() - 1; i >= 0; i--) {
    if (i == 0) {
      ofile << "v" + to_string(r.second[i] + 1) << " → "
            << "v1";
    } else {
      ofile << "v" + to_string(r.second[i] + 1) << " → ";
    }
  }
  ofile << endl << " Execution time: " << executionTime << " seconds" << endl;
  ofile.close();
  return 0;
}
