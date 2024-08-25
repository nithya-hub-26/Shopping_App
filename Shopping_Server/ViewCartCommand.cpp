#include "ViewCartCommand.h"

ViewCartCommand::ViewCartCommand(int client_id, unordered_map<int, vector<Product*>>& clientCarts) :
    client_id(client_id), clientCarts(clientCarts) {}

string ViewCartCommand::execute() {
    string response;
    if (clientCarts.find(client_id) != clientCarts.end()) {
        if (clientCarts[client_id].empty()) {
            response = "Cart is empty.";
            LOG_INFO(response);
            return response;
        }
        for (const auto& product : clientCarts[client_id]) {
            response += product->getDescription() + "\n";
        }
    }
    else {
        response = "Cart is empty.";
        LOG_INFO(response);
    }
    return response;
}