#include "dataHandler.hpp"

std::vector<std::vector<double>> DataHandler::load_data(const std::string& file_path) {
    std::vector<std::vector<double>> data;
    std::ifstream file(file_path);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        double value;

        while (ss >> value) {
            row.push_back(value);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }

        data.push_back(row);
    }

    return data;
}

std::vector<int> DataHandler::load_labels(const std::string& file_path) {
    std::vector<int> labels;
    std::ifstream file(file_path);
    std::string line;

    while (std::getline(file, line)) {
        int label;
        std::stringstream ss(line);
        ss >> label;
        labels.push_back(label);
    }

    return labels;
}


void DataHandler::format_training_data(const std::vector<std::string> &data, std::vector<std::vector<double>> &train_data, std::vector<int> &train_labels)
{
  for (auto& d : data)
  {
    std::vector<double> board(9);
    for (int i = 0; i < 9; i++)
    {
        board[i] = d[i] - '0';
    }
    train_data.push_back(board);
    train_labels.push_back(d[9]);
  }
}

std::vector<double> DataHandler::getBoard(std::string board)
{
    std::vector<double> b(9);
    for (int i = 0; i < 9; i++)
    {
        b[i] = board[i] - '0';
    }
    return b;
}

void DataHandler::normalize_data(std::vector<std::vector<double>>& data) {
    for (size_t j = 0; j < data[0].size(); ++j) {
        std::vector<double> column;
        for (size_t i = 0; i < data.size(); ++i) {
            column.push_back(data[i][j]);
        }

        double column_mean = mean(column);
        double column_std = std_dev(column, column_mean);

        for (size_t i = 0; i < data.size(); ++i) {
            data[i][j] = (data[i][j] - column_mean) / column_std;
        }
    }
}

double DataHandler::mean(const std::vector<double>& data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

double DataHandler::std_dev(const std::vector<double>& data, double mean) {
    double sum = 0.0;
    for (const auto& value : data) {
        sum += std::pow(value - mean, 2);
    }
    return std::sqrt(sum / data.size());
}
