#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cmath>

class DataHandler {
public:
  DataHandler() = default;
  std::vector<std::vector<double>> load_data(const std::string &file_path);
  std::vector<int> load_labels(const std::string &file_path);
  void normalize_data(std::vector<std::vector<double>> &data);

private:
  double mean(const std::vector<double> &data);
  double std_dev(const std::vector<double> &data, double mean);
};

#endif
