#ifndef PRODUCT_DECORATOR_H
#define PRODUCT_DECORATOR_H

#include "Product.h"

class ProductDecorator : public Product {
protected:
    Product* decoratedProduct;
public:
    ProductDecorator(Product* product) :  decoratedProduct(product->clone()){}

   int getProdId() {
        return decoratedProduct->getProdId();
   }
   string getDescription(){
        return decoratedProduct->getDescription();
   }
   double getPrice() {
       return decoratedProduct->getPrice();
   } 
   string getName() {
       return decoratedProduct->getName();
   }
   string getCategory() {
       return decoratedProduct->getCategory();
   }
   int getQuantity() {
       return decoratedProduct->getQuantity();
   }
   void reduceQuantity(int amount) {
       return decoratedProduct->reduceQuantity(amount);
   }
   bool changeQuantity(int amount) {
       return decoratedProduct->changeQuantity(amount);
   }
   void increaseQuantity(int amount) {
       return decoratedProduct->increaseQuantity(amount);
   }
   Product* clone() {   // Prototype Pattern 
       return new ProductDecorator(*this);
   }
};

#endif