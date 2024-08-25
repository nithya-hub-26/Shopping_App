#include "DeleteFromCartCommand.h"

DeleteFromCartCommand::DeleteFromCartCommand(vector<Product*>& products, int prodId, int client_id, unordered_map<int, vector<Product*>>& clientCarts) :
    products(products), productId(prodId), client_id(client_id), clientCarts(clientCarts) {}

string DeleteFromCartCommand::execute() {
    string response;
    try {
        if (clientCarts.find(client_id) != clientCarts.end()) {
            auto& cart = clientCarts[client_id];
            auto delItr = find_if(cart.begin(), cart.end(),
                [this](Product* product) { return product->getProdId() == productId;}); // lambda function
            if (delItr != cart.end()) {
                auto& delproduct = *delItr;
                products[delproduct->getProdId() - 1]->increaseQuantity(delproduct->getQuantity());
                cart.erase(delItr);
                response += "Deleted product with ID " + to_string(productId) + " from client " + to_string(client_id) + "'s cart.";
                LOG_INFO(response);
            }
            else {
                response += "Invalid product id. Product is not found in cart.";
                LOG_INFO("Client " + to_string(client_id) + " entered invalid product id.");
            }
        }
        else {
            response += "Cart does not exist for client " + to_string(client_id) + ".";
            LOG_INFO(response);
        }
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in Deletefromcart Command: " + string(e.what()));
        response = "Error occurred during command execution.";
    }
    return response;
}