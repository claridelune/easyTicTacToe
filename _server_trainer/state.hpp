#ifndef STATE_H
#define STATE_H

#include <string>

class TrainerClient;

class State {
public:
    virtual void handleConfig(TrainerClient* client, const std::string& config) = 0;
    virtual void handleAction(TrainerClient* client, const std::string& action) = 0;
    virtual ~State() = default;
};

#endif // STATE_H
