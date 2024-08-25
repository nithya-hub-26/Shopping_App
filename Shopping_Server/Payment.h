#include <memory>
#include "PaymentProcessor.h"

class Payment {
private:
    PaymentProcessor* processor;
public:
    Payment(PaymentProcessor* processor): processor(processor) {}

    bool processPayment(double amount) {
        return processor->processPayment(amount);
    }

    string getProcessorName() {
        return processor->getProcessorName();
    }
};