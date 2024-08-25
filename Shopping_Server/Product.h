#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
using namespace std;

class Product {
public:
    virtual int getProdId() = 0;
    virtual string getCategory() = 0;
    virtual string getName() = 0;
    virtual double getPrice() = 0;
    virtual int getQuantity() = 0;
    virtual bool changeQuantity(int amount) = 0;
    virtual void reduceQuantity(int amount) = 0;
    virtual void increaseQuantity(int amount) = 0;
    virtual string getDescription() = 0;
    virtual Product* clone() = 0; // Prototype Pattern
    virtual ~Product() {}
};

#endif 