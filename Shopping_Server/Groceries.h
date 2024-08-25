#ifndef GROCERIES_H
#define GROCERIES_H

#include "Product.h"
using namespace std;

// Abstract product
class Groceries: public Product{
    int prodId;
    string name;
    double price;
    int quantity;
    string category;

public:
    Groceries(int prodId, const string& category, const string& name, double price, int quantity) :
        prodId(prodId), category(category), name(name), price(price), quantity(quantity) {}

    int getProdId() {
        return prodId;
    }
    string getName() {
        return name;
    }
    string getCategory() {
        return category;
    }
    double getPrice() {
        return price;
    }
    int getQuantity() {
        return quantity;
    }
    void reduceQuantity(int amount) {
        quantity -= amount;
    }
    bool changeQuantity(int amount) {
        if (amount > quantity) {
            return false;
        }
        quantity = amount;
        return true;
    }
    void increaseQuantity(int amount) {
        quantity += amount;
    }
    string getDescription() {
        return to_string(prodId) + " " + name + ", Category: " + category +  ", Price: " + to_string(price) + ", Quantity : " + to_string(quantity);
    }
    Product* clone() {   // Prototype Pattern 
        return new Groceries(*this);
    }
};

#endif


