#ifndef VIEW_CART_COMMAND_H
#define VIEW_CART_COMMAND_H

#include "Command.h"

class ViewCartCommand : public Command {
private:
	unordered_map<int, vector<Product*>>& clientCarts;
	int client_id;

public:
    ViewCartCommand(int client_id, unordered_map<int, vector<Product*>>& clientCarts);

	string execute();
};

#endif
