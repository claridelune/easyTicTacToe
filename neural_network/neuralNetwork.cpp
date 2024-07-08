#include "neuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(unsigned int _n_inputs, unsigned int _n_hidden,
                             unsigned int _n_outputs, unsigned int _n_epochs,
                             double _learning_rate)
    : n_inputs(_n_inputs), n_hidden(_n_hidden), n_outputs(_n_outputs),
      n_epochs(_n_epochs), learning_rate(_learning_rate),
      gen(std::random_device()()) {
  std::normal_distribution<> d(0, 1);

  W1.resize(n_hidden, std::vector<double>(n_inputs));
  b1.resize(n_hidden);
  W2.resize(n_outputs, std::vector<double>(n_hidden));
  b2.resize(n_outputs);

  he_initialization(W1);
  he_initialization(W2);

  for (auto &row : W1)
    for (auto &w : row)
      w = d(gen);
  for (auto &row : W2)
    for (auto &w : row)
      w = d(gen);
  std::fill(b1.begin(), b1.end(), 0);
  std::fill(b2.begin(), b2.end(), 0);
}

void NeuralNetwork::he_initialization(std::vector < std::vector < double >> & W) {
  double mean = 0.0;
  double std = std::sqrt(2.0 / static_cast < double > (W.size()));
  std::normal_distribution < double > rand_normal(mean, std);
  for (unsigned int i = 0; i < W.size(); ++i) {
    for (unsigned int j = 0; j < W[0].size(); ++j) {
      W[i][j] = rand_normal(gen);
    }
  }
}

std::vector < double > NeuralNetwork::matmul(
  const std::vector < std::vector < double >> & W,
    const std::vector < double > & x,
      const std::vector < double > & b) {
  std::vector < double > z(W.size());
  for (size_t i = 0; i < W.size(); ++i) {
    z[i] = b[i];
    for (size_t j = 0; j < x.size(); ++j) {
      z[i] += W[i][j] * x[j];
    }
  }
  return z;
}

std::vector < double > NeuralNetwork::relu(const std::vector < double > & x) {
  std::vector < double > y(x.size());
  for (size_t i = 0; i < x.size(); ++i) {
    y[i] = std::max(0.0, x[i]);
  }
  return y;
}

std::vector < double > NeuralNetwork::softmax(const std::vector < double > & x) {
  std::vector < double > y(x.size());
  double sum_exp = 0.0;
  for (double xi: x) {
    sum_exp += std::exp(xi);
  }
  for (size_t i = 0; i < x.size(); ++i) {
    y[i] = std::exp(x[i]) / sum_exp;
  }
  return y;
}

void NeuralNetwork::forward(const std::vector < double > & x,
  std::vector < double > & z1, std::vector < double > & a1,
  std::vector < double > & z2, std::vector < double > & a2) {
  z1 = matmul(W1, x, b1);
  a1 = relu(z1);
  z2 = matmul(W2, a1, b2);
  a2 = softmax(z2);
}

void NeuralNetwork::backpropagate(const std::vector < double > & x,
  const std::vector < double > & y,
    std::vector < double > & z1,
    std::vector < double > & a1,
    std::vector < double > & z2,
    std::vector < double > & a2) {
  std::vector < double > delta2(n_outputs);
  for (size_t i = 0; i < n_outputs; ++i) {
    delta2[i] = a2[i] - y[i];
  }

  std::vector < double > delta1(n_hidden, 0.0);
  for (size_t i = 0; i < n_hidden; ++i) {
    for (size_t j = 0; j < n_outputs; ++j) {
      delta1[i] += W2[j][i] * delta2[j] * (z1[i] > 0 ? 1.0 : 0.0);
    }
  }

  for (size_t i = 0; i < n_outputs; ++i) {
    for (size_t j = 0; j < n_hidden; ++j) {
      W2[i][j] -= learning_rate * delta2[i] * a1[j];
    }
    b2[i] -= learning_rate * delta2[i];
  }

  for (size_t i = 0; i < n_hidden; ++i) {
    for (size_t j = 0; j < n_inputs; ++j) {
      W1[i][j] -= learning_rate * delta1[i] * x[j];
    }
    b1[i] -= learning_rate * delta1[i];
  }
}

void NeuralNetwork::train(std::vector < std::vector < double >> data_train,
  std::vector < int > labels_train) {
  for (unsigned int epoch = 0; epoch < n_epochs; ++epoch) {
    for (size_t i = 0; i < data_train.size(); ++i) {
      std::vector < double > y(n_outputs, 0.0);
      y[labels_train[i]] = 1.0;

      std::vector < double > z1, a1, z2, a2;
      forward(data_train[i], z1, a1, z2, a2);
      backpropagate(data_train[i], y, z1, a1, z2, a2);
    }
  }
}

void NeuralNetwork::run(std::vector < std::vector < double >> data_train,
  std::vector < int > labels_train) {
  for (size_t i = 0; i < data_train.size(); ++i) {
    std::vector < double > y(n_outputs, 0.0);
    y[labels_train[i]] = 1.0;

    std::vector < double > z1, a1, z2, a2;
    forward(data_train[i], z1, a1, z2, a2);
    backpropagate(data_train[i], y, z1, a1, z2, a2);
  }
}

std::vector<double> NeuralNetwork::predict(const std::vector < double > & x) {
  std::vector < double > z1, a1, z2, a2;
  forward(x, z1, a1, z2, a2);
  return a2;
}
