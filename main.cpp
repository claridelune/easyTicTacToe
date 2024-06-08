#include "dataHandler.h"
#include "neuralNetwork.h"
#include <iostream>

int main() {
    DataHandler dataHandler;
    NeuralNetwork nn(9, 5, 9, 30, 0.01);

    std::vector<std::vector<double>> train_data = dataHandler.load_data("train_data.csv");
    std::vector<int> train_labels = dataHandler.load_labels("train_labels.csv");
    dataHandler.normalize_data(train_data);
    nn.train(train_data, train_labels);

    for (auto i: train_data) {
      int best_move = nn.predict(i);
      std::cout << std::endl;
      std::cout << "La mejor jugada es en la posición: " << best_move << std::endl;
    }

    return 0;
}
