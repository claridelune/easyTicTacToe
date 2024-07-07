#include <iostream>
#include "trainer.hpp"
#include "../shared/utils.hpp"

#include "../udp/udp.hpp"
#include "../neural_network/dataHandler.hpp"
#include "../neural_network/neuralNetwork.hpp"

#define IP "127.0.0.1"
#define UDP_PORT "3490"
#define TCP_CLIENT_PORT 4490
#define TCP_SERVER_PORT 5490
#define ROL 3

int currSeq = 0;

std::vector<std::string> data;

bool evaluateDatum(const std::string& datum)
{
    if (!checkSum(datum) || (datum[0] != 'd' && datum[0] != 'f'))
        return false;

    if (datum[0] == 'f')
        return true;

    int seq = binaryToInt(datum.substr(1, 4));
    return currSeq == seq;
}

int main() {
    Logger logger("TrainerServer");
    try {
        TrainerClient trainer("127.0.0.1", TCP_CLIENT_PORT, &logger);

        Request requestJoin;
        requestJoin.action = "join";
        requestJoin.credential = {{"role", ROL}, {"name", uuid()}};
        requestJoin.data = {};

        trainer.sendRequest(requestJoin);
        trainer.listen();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    // Receive Data
    
    UDPTalker talker(2, IP, UDP_PORT, &evaluateDatum);

    for (; true; currSeq++)
    {
        std::string message;
        message.push_back('r');
        message += intToBinary(currSeq);

        int cs = getSum(message);
        message.push_back(cs);

        auto resp = talker.sendNReceive(message);

        if (resp[0] == 'f')
            break;
        
        data.push_back(resp.substr(5, 10));
    }

    // Training
    
    /* DataHandler dataHandler;
    NeuralNetwork nn(9, 5, 9, 30, 0.01);
    
    std::vector<std::vector<double>> train_data;
    std::vector<int> train_labels;
    dataHandler.format_training_data(data, train_data, train_labels);
    dataHandler.normalize_data(train_data); */

    /*
    * Entrenamiento de la red neuronal
    * Sin tomar en cuenta las epocas de los parametros
    * Solo 1 forward y 1 backpropagate
    * */
    
    // nn.run(train_data, train_labels);

    /*
    * Procedimiento de los pesos
    * Pasar los pesos
    * Actualizar los pesos
    * */

    // std::string weights = dataHandler.vector_to_binary_string(nn.W1, nn.W2);
    // dataHandler.binaryStringToVector(weights, nn.W1, nn.W2);

    /*
    * Uso con el cliente
    * Predict devuelve el casillero donde va a mover en move
    * */

    /* std::string board = "000000000";
    int move = nn.predict(dataHandler.getBoard(board));

    std::cout << move << '\n'; */

    return 0;
}

