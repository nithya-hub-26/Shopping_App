#include "ApplyDiscountCommand.h"

ApplyDiscountCommand::ApplyDiscountCommand(int client_id, unordered_map<int, vector<Product*>>& clientCarts) :
    client_id(client_id), clientCarts(clientCarts) {}

string ApplyDiscountCommand::execute() {
    string response;
    try {
        bool discountproduct_flag = false;
        if (clientCarts.find(client_id) != clientCarts.end()) {
            for (auto& product : clientCarts[client_id]) {
                // If product is not a wrapped with discount decorator, dynamic_cast will return nullptr
                DiscountDecorator* decorator = dynamic_cast<DiscountDecorator*>(product);   // Used for downcasting from product to discount decorator 
                if (product->getPrice() > 1000 && decorator == nullptr) {
                    product = new DiscountDecorator(product);
                    discountproduct_flag = true;
                }
            }

            if (!discountproduct_flag)
                response = "No qualifying products are available for discount in cart";
            else
                response = "Discount applied to qualifying products in cart.";
        }
        else {
            response = "Cart is empty.";
        }
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in Applydiscount Command: " + string(e.what()));
        response = "Error occurred during command execution.";
    }
    LOG_INFO(response);
    return response;
}