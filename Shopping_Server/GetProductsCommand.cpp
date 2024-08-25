#include "GetProductsCommand.h"

GetProductsCommand::GetProductsCommand(vector<Product*>& products) : products(products) {}

string GetProductsCommand::execute() {
    string response;
    for (const auto& product : products) {
        response += product->getDescription() + "\n";
    }
    return response;
}