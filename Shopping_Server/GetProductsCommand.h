#ifndef GET_PRODUCTS_COMMAND_H
#define GET_PRODUCTS_COMMAND_H

#include "Command.h"

class GetProductsCommand : public Command {
private:
    vector<Product*>& products;

public:
    GetProductsCommand(vector<Product*>& products);

    string execute();
};

#endif