#ifndef DISCOUNT_DECORATOR_H
#define DISCOUNT_DECORATOR_H

#include "ProductDecorator.h"

class DiscountDecorator : public ProductDecorator {
private:
    double discountPercentage = 5;

public:
    DiscountDecorator(Product* product): ProductDecorator(product) {}

    string getDescription(){
        return decoratedProduct->getDescription() + " (Discounted: " + to_string(discountPercentage) + "% off, Price after discount: " + to_string(decoratedProduct->getPrice() * (1 - discountPercentage/100)) + ")";
    }
    
    double getPrice() {
        return decoratedProduct->getPrice() * (1 - discountPercentage/100);
    }

    Product* clone() {   // Prototype Pattern 
        return new DiscountDecorator(*this);
    }
};

#endif