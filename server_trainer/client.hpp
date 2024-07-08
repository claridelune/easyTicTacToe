#ifndef SERVER_TRAINER_CLIENT_H
#define SERVER_TRAINER_CLIENT_H

#include "processor.hpp"

#include "../shared/socket.hpp"

class TrainerClient : public TrainerProcessor {
    private:
        ProcessorOpts _extraOptions;

        bool _requiredServerInstance;
        bool _requiredDisposed;
        bool _requiredServerInstanceFirstTime;

    public:
        bool receiveData;

        TrainerClient(Socket* socket, ProcessorOpts& opts): TrainerProcessor(socket) {
            setOptions(opts);
            receiveData = false;
        }  

        ProcessorOpts& getExtraOptions() { return _extraOptions; }
        void setExtraOptions(ProcessorOpts opts) { _extraOptions = opts; }

        bool requiredServerInstance() { return _requiredServerInstance;}
        bool requiredDisposed() { return _requiredDisposed; }
        bool requiredServerInstanceFirstTime() { return _requiredServerInstanceFirstTime; }

        void setRequiredServerInstance(bool v) { _requiredServerInstance = v;}
        void setRequiredDisposed(bool v) { _requiredDisposed = v; }
        void setRequiredServerInstanceFirstTime(bool v) { _requiredServerInstanceFirstTime = v; }

        void initialize() override;
        void configure() override;

        void config(Request req);
        Response predict(Request req);
        Response keepAlive(Request req);
        Response data(Request req);
};

#endif
