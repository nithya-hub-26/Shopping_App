#ifndef CARD_PROCESSOR_H
#define CARD_PROCESSOR_H

#include "PaymentProcessor.h"

class CardProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) {
        return true; // Assuming payment is successful
    }

    string getProcessorName() {
        return "Card Payment";
    }
};

#endif