#ifndef UPI_PROCESSOR_H
#define UPI_PROCESSOR_H

#include "PaymentProcessor.h"

class UPIProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) {
        return true; // Assuming payment is successful
    }

    string getProcessorName() {
        return "UPI Payment";
    }
};

#endif