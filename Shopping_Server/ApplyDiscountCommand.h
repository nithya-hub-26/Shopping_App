#ifndef APPLY_DISCOUNT_COMMAND_H
#define APPLY_DISCOUNT_COMMAND_H

#include "Command.h"
#include "DiscountDecorator.h"

class ApplyDiscountCommand : public Command {
private:
    int client_id;
    unordered_map<int, vector<Product*>>& clientCarts;
public:
    ApplyDiscountCommand(int client_id, unordered_map<int, vector<Product*>>& clientCarts);

    string execute();
};

#endif