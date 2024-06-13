#include "common.h"
#include "dataHandler.h"
#include "neuralNetwork.h"


int main()
{

  /*
   * Considerando que que ya se envio la data del server principal
   * */
  std::string data;


  DataHandler dataHandler;
  NeuralNetwork nn(9, 5, 9, 30, 0.01);
  
  std::vector<std::vector<double>> train_data;
  std::vector<int> train_labels;
  dataHandler.format_training_data(data, train_data, train_labels);
  dataHandler.normalize_data(train_data);

  /*
   * Entrenamiento de la red neuronal
   * Sin tomar en cuenta las epocas de los parametros
   * Solo 1 forward y 1 backpropagate
   * */
  
  nn.run(train_data, train_labels);

  /*
   * Procedimiento de los pesos
   * Pasar los pesos
   * Actualizar los pesos
   * */

  std::string weights = dataHandler.vector_to_binary_string(nn.W1, nn.W2);
  dataHandler.binaryStringToVector(weights, nn.W1, nn.W2);

  /*
   * Uso con el cliente
   * Predict devuelve el casillero donde va a mover en move
   * */

  std::string board;
  int move = nn.predict(dataHandler.getBoard(board));

  /*yyy bueno asi seria su uso asldgjkasdlg*/

  return 0;
}   
