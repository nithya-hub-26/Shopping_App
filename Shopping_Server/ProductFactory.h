#ifndef PRODUCTFACTORY_H
#define PRODUCTFACTORY_H

#include "Clothing.h"
#include "Accessories.h"
#include "Groceries.h"


class ProductFactory {
public:
	virtual Product* createProduct(int prodId, const string& category, const string& name, double price, int quantity) = 0;
    virtual ~ProductFactory(){}
};

class GroceriesFactory: public ProductFactory{
	Product* createProduct(int prodId, const string& category, const string& name, double price, int quantity) {
		return new Groceries(prodId, category, name, price, quantity);
	}
};

class ClothingFactory : public ProductFactory {
	Product* createProduct(int prodId, const string& category, const string& name, double price, int quantity) {
		return new Clothing(prodId, category, name, price, quantity);
	}
};

class AccessoriesFactory : public ProductFactory {
	Product* createProduct(int prodId, const string& category, const string& name, double price, int quantity) {
		return new Accessories(prodId, category, name, price, quantity);
	}
};

#endif





