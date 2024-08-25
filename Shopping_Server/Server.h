#ifndef SERVER_H
#define SERVER_H

// Including necessary headers and libraries for server functionality
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")   // Linking Windows Socket library
// Including custom command and product classes
#include "ProductFactory.h"
#include "GetProductsCommand.h"
#include "AddToCartCommand.h"
#include "ViewCartCommand.h"
#include "ApplyDiscountCommand.h"
#include "DeleteFromCartCommand.h"
#include "CheckoutCommand.h"
#include "PaymentCommand.h"
#include "CommandMenu.h"
#include "DiscountDecorator.h"
#include "PaymentProcessor.h"
#include "Logger.h"
#include "User.h"
using namespace std;

// Server class declaration
class Server {
public:
    static Server& getInstance(int tcpPort, int udpPort, string productsFile);

    //Disabling copy and assignment for server class
    Server(const Server&) = delete;
    void operator=(const Server&) = delete;

    // Function to start the server's main functionality
    void start();

private:
    Server(int tcpPort, int udpPort, string productsFile);
    ~Server();

    int tcpPort;
    int udpPort;
    string productsFile;
    unordered_map<int, User> Users;
    unordered_map<int, bool> clientLoginStatus; 
    unordered_map<int, vector<Product*>> clientCarts;  // Map to store client carts with client ID as key
    vector<Product*> products;                         // Vector to store all available products
    mutex mtx;                                         // Mutex for thread synchronization
    CommandMenu cmdMenu;                               // Command menu to execute commands

    void initProducts();
    void saveProductsToFile(vector<Product*>& products, string& productsFile);
    void loadClientsFromCSV();
    void startTCPServer();
    void startUDPServer();
    void broadcastMessage(int udpSocket, sockaddr_in broadcastAddr);
    void sendresponse(int client_socket, string response);
    string recvrequest(int client_socket, int user_id);
    int productIdRequest(int client_socket, int user_id);
    void cartCheck(int client_id);
    string getMenu();
    string handleRequest(string& request, int client_id, int user_id);
    void handleClient(int client_socket);
    void registerNewClient(int user_id);
    void saveClientData(int user_id);
};

// thread id
// data files
// each table in each file
// clientid in map only after register
// client to users and accounts
// register client
// init products in file no reset
#endif