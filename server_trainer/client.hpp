#ifndef SERVER_TRAINER_CLIENT_H
#define SERVER_TRAINER_CLIENT_H

#include "processor.hpp"

#include "../shared/socket.hpp"
#include "../neural_network/neuralNetwork.hpp"

class TrainerClient : public TrainerProcessor {
    private:
        ProcessorOpts _extraOptions;

        bool _requiredServerInstance;
        bool _requiredDisposed;
        bool _requiredServerInstanceFirstTime;
        bool _receiveData = false;


    public:
        TrainerClient(Socket* socket, NeuralNetwork *nn, ProcessorOpts& opts): TrainerProcessor(socket, nn) {
            setOptions(opts);
        }  

        ProcessorOpts& getExtraOptions() { return _extraOptions; }
        void setExtraOptions(ProcessorOpts opts) { _extraOptions = opts; }

        bool requiredServerInstance() { return _requiredServerInstance;}
        bool requiredDisposed() { return _requiredDisposed; }
        bool requiredServerInstanceFirstTime() { return _requiredServerInstanceFirstTime; }
        bool receiveData() { return _receiveData; }

        void setRequiredServerInstance(bool v) { _requiredServerInstance = v;}
        void setRequiredDisposed(bool v) { _requiredDisposed = v; }
        void setRequiredServerInstanceFirstTime(bool v) { _requiredServerInstanceFirstTime = v; }
        void setReceiveData(bool v) { _receiveData = v; }

        void initialize() override;
        void configure() override;

        void config(Request req);
        void join(Request req);
        void data(Request req);
        void train(Request req);
        Response predict(Request req);
        Response keepAlive(Request req);

        void initializeTrainingData(std::vector<std::string>& data);
};

#endif
