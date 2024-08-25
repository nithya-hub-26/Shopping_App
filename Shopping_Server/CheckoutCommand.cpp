#include "CheckoutCommand.h"

CheckoutCommand::CheckoutCommand() : client_id(0), clientCarts(clientCarts) {}

CheckoutCommand::CheckoutCommand(int client_id, unordered_map<int, vector<Product*>>& clientCarts) :
	client_id(client_id), clientCarts(clientCarts) {}

string CheckoutCommand::execute() {
	string response;
	try {
		if (clientCarts.find(client_id) != clientCarts.end()) {
			if (clientCarts[client_id].empty()) {
				response = "Cart is empty.\n";
				return response;
			}
			for (const auto& product : clientCarts[client_id]) {
				totalAmount += product->getPrice() * product->getQuantity();
				response += product->getDescription() + "\n";
			}
			response += "\nTotal amount: " + to_string(totalAmount) + "\n";
		}
		else {
			response = "Cart is empty.\n";
		}
	}
	catch (const exception& e) {
		LOG_ERROR("Exception caught in Checkout Command: " + string(e.what()));
		response = "Error occurred during command execution.";
	}
	return response;
}

double CheckoutCommand::gettotalAmount() {
	return totalAmount;
}