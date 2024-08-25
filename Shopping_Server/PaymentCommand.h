#ifndef PAYMENT_COMMAND_H
#define PAYMENT_COMMAND_H

#include "Command.h"
#include "Payment.h"
#include "PaymentProcessor.h"
#include "CardProcessor.h"
#include "UPIProcessor.h"
#include "AddToCartCommand.h"

class PaymentCommand : public Command {
private:
    PaymentProcessor* processor;
    char paymentChoice;
    double totalAmount;
public:
    PaymentCommand(PaymentProcessor* processor, char paymentChoice, double totalAmount);

    string execute();
};

#endif