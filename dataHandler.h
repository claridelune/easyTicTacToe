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

  void format_training_data(const std::string &data, std::vector<std::vector<double>> &train_data, std::vector<int> &train_labels);
  std::string vector_to_binary_string(const std::vector<std::vector<double>>& w1, const std::vector<std::vector<double>>& w2);
  void binaryStringToVector(std::string weights, std::vector<std::vector<double>>& w1, std::vector<std::vector<double>>& w2);
  std::vector<double> getBoard(std::string board);

private:
  double mean(const std::vector<double> &data);
  double std_dev(const std::vector<double> &data, double mean);
};

#endif
