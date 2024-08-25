#ifndef ADD_TO_CART_COMMAND_H
#define ADD_TO_CART_COMMAND_H

#include "Command.h"

class AddToCartCommand : public Command {
private:
    unordered_map<int, vector<Product*>>& clientCarts;
    vector<Product*>& products;
    int productIndex, client_id, quantity;
public:
    AddToCartCommand(int prodId, vector<Product*>& products, int quantity, int client_id, unordered_map<int, vector<Product*>>& clientCarts);

    string execute();
    void rollback();
};


#endif