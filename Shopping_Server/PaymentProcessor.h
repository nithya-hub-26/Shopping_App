#ifndef PAYMENT_PROCESSOR_H
#define PAYMENT_PROCESSOR_H

#include <string>
using namespace std;

class PaymentProcessor {
public:
    virtual bool processPayment(double amount) = 0;
    virtual string getProcessorName() = 0;
    virtual ~PaymentProcessor() {}
};

#endif