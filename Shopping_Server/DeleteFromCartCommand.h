#ifndef DELETE_FROM_CART_COMMAND_H
#define DELETE_FROM_CART_COMMAND_H

#include "Command.h"

class DeleteFromCartCommand : public Command {
private:
    unordered_map<int, vector<Product*>>& clientCarts;
    vector<Product*>& products;
    int productId, client_id;
public:
    DeleteFromCartCommand(vector<Product*>& products, int prodId, int client_id, unordered_map<int, vector<Product*>>& clientCarts);

    string execute();
};
#endif