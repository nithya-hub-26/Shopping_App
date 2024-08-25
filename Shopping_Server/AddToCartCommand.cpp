#include "AddToCartCommand.h"

AddToCartCommand::AddToCartCommand(int prodId, vector<Product*>& products, int quantity, int client_id, unordered_map<int, vector<Product*>>& clientCarts) :
    productIndex(prodId - 1), products(products), quantity(quantity), client_id(client_id), clientCarts(clientCarts) {}

string AddToCartCommand::execute() {
    string response;
    try {
        // Simulate adding product to cart to client id
        Product* productToAdd = products[productIndex]->clone(); // Clone the product from products
        if (!productToAdd->changeQuantity(quantity)) {
            response = "This product is out of stock.\n";
            LOG_INFO(string(productToAdd->getName()) + " is out of stock");
            return response;
        }

        if (clientCarts.find(client_id) != clientCarts.end()) {
            bool newProductFlag = true;
            for (auto& product : clientCarts[client_id]) {
                if (productToAdd->getProdId() == product->getProdId()) {
                    product->increaseQuantity(quantity);
                    newProductFlag = false;
                    break;
                }
            }
            if (newProductFlag) {
                clientCarts[client_id].push_back(productToAdd); // Add product to client's cart
            }
            response = "Added product to client " + to_string(client_id) + "'s cart.";
            LOG_INFO(response);
        }
        else {
            vector<Product*> newCart;
            newCart.push_back(productToAdd); // Clone product to client's cart
            clientCarts.emplace(client_id, move(newCart));
            response = "Added product to new cart for client " + to_string(client_id) + ".";
            LOG_INFO(response);
        }
        products[productIndex]->reduceQuantity(quantity);
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in Addtocart Command: " + string(e.what()));
        response = "Error occurred during command execution.";
    }

    return response;
}

void AddToCartCommand::rollback() {
    if (productIndex >= 0 && productIndex < products.size()) {
        products[productIndex]->increaseQuantity(quantity);
    }
}