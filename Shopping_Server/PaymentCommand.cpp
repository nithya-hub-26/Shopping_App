#include "PaymentCommand.h"

PaymentCommand::PaymentCommand(PaymentProcessor* processor, char paymentChoice, double totalAmount) :
    processor(processor), paymentChoice(paymentChoice), totalAmount(totalAmount) {}

string PaymentCommand::execute() {
    string response;

    try {
        if (paymentChoice == 'C' || paymentChoice == 'c') {
            processor = new CardProcessor();
        }
        else if (paymentChoice == 'U' || paymentChoice == 'u') {
            processor = new UPIProcessor();
        }
        else {
            response = "Invalid payment method choice, transaction failed.";
            LOG_INFO("Client entered invalid payment method choice");
            LOG_INFO("Payment is unsuccessful");

            return response;
        }

        Payment payment(processor);
        payment.processPayment(totalAmount);
        response = "Payment successful using " + payment.getProcessorName() + "!!!";
        LOG_INFO(response);
        response += "\nThank you for purchasing.If you want you can continue shopping.\n";
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in payment processing: " + string(e.what()));
        response = "Error occurred during payment processing.";
        LOG_INFO("Payment is unsuccessful");
    }

    return response;
}