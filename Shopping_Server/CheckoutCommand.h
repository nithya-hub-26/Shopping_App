#ifndef CHECKOUT_COMMAND_H
#define CHECKOUT_COMMAND_H

#include "Command.h"

class CheckoutCommand : public Command {
private:
	double totalAmount = 0.0;
	int client_id;
	unordered_map<int, vector<Product*>>& clientCarts;
public:
	// Default constructor
	CheckoutCommand();
	CheckoutCommand(int client_id, unordered_map<int, vector<Product*>>& clientCarts);

	string execute();
	double gettotalAmount();
};

#endif
