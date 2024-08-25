#ifndef CLOTHING_H
#define CLOTHING_H

#include "Product.h"

class Clothing : public Product {
    int prodId;
    string category;
    string name;
    double price;
    int quantity;

public:
    Clothing(int prodId, const string& category, const string& name, double price, int quantity)
        : prodId(prodId), category(category), name(name), price(price), quantity(quantity){}

    int getProdId() {
        return prodId;
    }
    string getName(){
        return name; 
    }
    string getCategory() {
        return category;
    }
    double getPrice(){
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
    Product* clone(){
        return new Clothing(*this); 
    }
    string getDescription(){
        return to_string(prodId) + " " + name + ", Category: " + category + ", Price: " + to_string(price) + ", Quantity: " + to_string(quantity);
    }
};

#endif





