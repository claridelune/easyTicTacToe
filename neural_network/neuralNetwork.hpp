#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>

#include <random>

#include <cmath>

#include <algorithm>

class NeuralNetwork {
public:
  NeuralNetwork(unsigned int _n_inputs, unsigned int _n_hidden,
                unsigned int _n_outputs, unsigned int _n_epochs,
                double _learning_rate);
  void train(std::vector<std::vector<double>> data_train,
             std::vector<int> labels_train);
  void run(std::vector<std::vector<double>> data_train,
             std::vector<int> labels_train);
  void forward(const std::vector<double> &x, std::vector<double> &z1,
               std::vector<double> &a1, std::vector<double> &z2,
               std::vector<double> &a2);
  std::vector<double> predict(const std::vector<double> &x);

  std::mt19937 gen;
  std::vector<std::vector<double>> W1;
  std::vector<std::vector<double>> W2;

private:
  unsigned int n_epochs;
  unsigned int n_inputs;
  unsigned int n_hidden;
  unsigned int n_outputs;
  double learning_rate;

  std::vector<double> b1;
  std::vector<double> b2;

  void he_initialization(std::vector<std::vector<double>> &W);
  std::vector<double> matmul(const std::vector<std::vector<double>> &W,
                             const std::vector<double> &x,
                             const std::vector<double> &b);
  std::vector<double> relu(const std::vector<double> &x);
  std::vector<double> softmax(const std::vector<double> &x);
  void backpropagate(const std::vector<double> &x, const std::vector<double> &y,
                     std::vector<double> &z1, std::vector<double> &a1,
                     std::vector<double> &z2, std::vector<double> &a2);
};

#endif
