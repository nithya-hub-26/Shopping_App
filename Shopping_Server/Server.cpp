#include "Server.h"

// Singleton pattern implementation for Server class
Server& Server::getInstance(int tcpPort, int udpPort, string productsFile) {
    // Static instance of Server, created only once and reused thereafter
	static Server instance(tcpPort, udpPort, productsFile);
	return instance;
}

// Constructor for the Server class
Server::Server(int tcpPort, int udpPort, string productsFile)    // Initialize tcpPort and udpPort with the provided values 
    : tcpPort(tcpPort), udpPort(udpPort), productsFile(productsFile) {

    initProducts();     // Call the function to initialize the products
    saveProductsToFile(products, productsFile);
    loadClientsFromCSV();
}

Server::~Server() {
    saveProductsToFile(products, productsFile);
    for (Product* product : products) {
        delete product;
    }
}

// Function to start the server
void Server::start() {
    try {
        // Start TCP and UDP servers in separate threads for concurrent execution
        thread tcpThread(&Server::startTCPServer, this);
        thread udpThread(&Server::startUDPServer, this);
        tcpThread.join();
        udpThread.join();
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in server start: " + string(e.what()));
    }
}

// Function to initialize the list of products
void Server::initProducts() {
    try {
        // Create factories for different product categories
        ProductFactory* groceries_factory = new GroceriesFactory();
        ProductFactory* clothing_factory = new ClothingFactory();
        ProductFactory* accessories_factory = new AccessoriesFactory();

        // Add products to the list using the factories
        products.push_back(groceries_factory->createProduct(1, "Fruits", "Apple", 200, 5));
        products.push_back(groceries_factory->createProduct(2, "Fruits", "Orange", 299.99, 4));
        products.push_back(groceries_factory->createProduct(3, "Vegetables", "Carrot", 199.99, 3));
        products.push_back(groceries_factory->createProduct(4, "Snacks", "Chips", 40, 2));


        products.push_back(clothing_factory->createProduct(5, "Gents", "Pants", 1499.99, 5));
        products.push_back(clothing_factory->createProduct(6, "Gents", "Shirts", 399.99, 4));
        products.push_back(clothing_factory->createProduct(7, "Gents", "Belt", 199.99, 2));
        products.push_back(clothing_factory->createProduct(8, "Ladies", "Skirts", 550, 6));
        products.push_back(clothing_factory->createProduct(9, "Ladies", "Saree", 1500, 4));

        products.push_back(accessories_factory->createProduct(10, "Gents", "Glasses", 150, 8));
        products.push_back(accessories_factory->createProduct(11, "Ladies", "Watch", 1400, 7));
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in loading products: " + string(e.what()));
    }
}

void Server::saveProductsToFile(vector<Product*>& products, string& productsFile) {
    ofstream file(productsFile);
    file << "Product_ID,Category,Product_Name,Price,Quantity\n";
    for (Product* product : products) {
        file << product->getProdId() << ','
            << product->getCategory() << ','
            << product->getName() << ','
            << product->getPrice() << ','
            << product->getQuantity() << '\n';
    }
}

void Server::loadClientsFromCSV() {
    lock_guard<mutex> lock(mutex);

    ifstream file("clients.csv");
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id_str, uname, pwd;

        getline(ss, id_str, ',');
        getline(ss, uname, ',');
        getline(ss, pwd, ',');

        int id = stoi(id_str);
        // Create a User object
        User user(id, uname, pwd);
        // Insert into the unordered map
        Users[id] = user;
    }
    file.close();
}

// TCP server start function
void Server::startTCPServer() {
    try {
        WSADATA wsaData;
        // Initializes Winsock with version request of 2.2
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw runtime_error("WSAStartup failed : " + to_string(result));
        }

        // Creates a TCP socket
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            throw runtime_error("Socket creation failed");
        }

        // Defines the address and port for server
        sockaddr_in address;
        address.sin_family = AF_INET;   //Sets address family to IPv4
        address.sin_addr.s_addr = INADDR_ANY;  // Sets IP address to any available interface
        address.sin_port = htons(tcpPort);  // Sets port number to 8080

        // Binds the socket to the IP address
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            closesocket(server_fd);
            throw runtime_error("Bind failed");
        }

        //Listens for incoming connections with max 3 reqs in backlog
        if (listen(server_fd, 3) < 0) {
            closesocket(server_fd);
            throw runtime_error("Listen failed");
        }

        LOG_INFO("TCP server started on port " + to_string(tcpPort));

        // Accepts new connections in a loop and starts a new thread to handle each client 
        while (true) {
            SOCKET new_socket = accept(server_fd, nullptr, nullptr);
            if (new_socket < 0) {
                LOG_WARN("Accept interrupted or connection aborted");
                continue;
            }
            thread(&Server::handleClient, this, new_socket).detach();
        }
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in TCP Server: " + string(e.what()));
        WSACleanup();
    }
}

// UDP server start function
void Server::startUDPServer() {
    try {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw runtime_error("WSAStartup failed : " + to_string(result));
        }

        // Creates a UDP socket
        int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (udpSocket == -1) {
            throw runtime_error("Socket creation failed");
        }

        // Set socket to broadcast mode to enable broadcasting in server
        int broadcastEnable = 1;
        if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) < 0) {
            closesocket(udpSocket);
            throw runtime_error("Failed to set socket to broadcast mode");
        }

        // Defines the broadcast address and port for server
        sockaddr_in broadcastAddr;
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
        broadcastAddr.sin_port = htons(udpPort);

        Sleep(500);
        LOG_INFO("UDP server started on port " + to_string(udpPort));
        cout << "\n";

        //Broadcast message is displayed for client who have loggedin
        Sleep(65000);
        LOG_INFO("Broadcasting message to all clients....");

        broadcastMessage(udpSocket, broadcastAddr);

        closesocket(udpSocket);
        WSACleanup();
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in UDP Server: " + string(e.what()));
    }
}

void Server::broadcastMessage(int udpSocket, sockaddr_in broadcastAddr) {
    string message = "5% off for products more than 1000 rupees ";
    for (auto& client : clientLoginStatus) {
        if (client.second) {
            int broadcastresult = sendto(udpSocket, message.c_str(), message.length(), 0, (struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
            if (broadcastresult < 0) {
                LOG_WARN("Broadcast message failed");
            }
        }
    }
}

// Server response function
void Server::sendresponse(int client_socket, string response) {

    // If the response string is empty, it returns null
    if (response.empty())
        return;

    // Attempt to send the response to the client
    try {
        if (send(client_socket, response.c_str(), response.length(), 0) == SOCKET_ERROR) {
            throw runtime_error("Send Failed");
        }
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in sending response: " + string(e.what()));
        closesocket(client_socket);
        WSACleanup();
        return;
    }
}

// Server request reception function
string Server::recvrequest(int client_socket, int user_id) {
    // Buffer to store the incoming request
    char buffer[1024];
    // Initialize the buffer with zeros
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    // If no bytes are read, the client has disconnected
    if (bytesRead <= 0) {
        LOG_WARN("Client " + Users[user_id].getUsername() + " disconnected\n");
        cartCheck(user_id);
        closesocket(client_socket);
        // Return an empty string to indicate disconnection
        return "";
    }
    // Null-terminate the received string
    buffer[bytesRead] = '\0';
    // Convert the buffer to a C++ string and return it
    string request(buffer);
    return request;
}

// Function to check and update the cart for a client
void Server::cartCheck(int user_id) {
    if (!clientCarts.empty()) {
        for (auto& delproduct : clientCarts[user_id]) {
            // Increase the quantity of the product in the inventory
            products[delproduct->getProdId() - 1]->increaseQuantity(delproduct->getQuantity());
        }
        saveProductsToFile(products, productsFile);
    }
}

// Function to request and validate a product ID from the client
int Server::productIdRequest(int client_socket, int user_id) {
    // Prompt the client to enter a product ID
    string response = "Enter Product Id: \n";
    sendresponse(client_socket, response);
    string request = recvrequest(client_socket, user_id);
    if (request.empty())
        return 0;
    // Extract the product ID from the client's response
    string idStr = request.substr(0);
    int productId = stoi(idStr);
    // Check if productId is valid
    if (productId > 0 && productId <= products.size()) {
        return productId;
    }
    return -1;
}

// Shopping menu display function
string Server::getMenu() {
    return "\nShopping Main Menu:\n\n\
 1. View products\n\
 2. Add product to cart\n\
 3. View cart\n\
 4. Delete product from cart\n\
 5. Apply discount to cart\n\
 6. Checkout the cart\n\
 7. Logout\n\
 8. Exit\n\
 \nEnter your choice (1-8): ";
}

// Handles client requests and generates appropriate responses
string Server::handleRequest(string& request, int client_id, int user_id) {
    // Lock to access the shared resources
    lock_guard<mutex> lock(mtx);

    string response;

    try {
        if (request == "0") {
            response = getMenu();
        }
        else if (request == "1") {
            cmdMenu.addCommand(new GetProductsCommand(products));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is viewing all available products");
            response = cmdMenu.executeCommand();
        }
        else if (request == "2") {
            int productId = productIdRequest(client_id, user_id);
            // If no product ID is provided, return empty string
            if (productId == 0)
                return "";
            // If an invalid product ID is provided, inform the client
            else if (productId == -1) {
                LOG_INFO("Client " + Users[user_id].getUsername() + " entered invalid product id. Product is not available");
                response = "Invalid product Id. Product is not available.";
                return response;
            }
            // Ask the client for the quantity of the product
            response = "Enter the quantity of product: \n";
            sendresponse(client_id, response);
            request = recvrequest(client_id, user_id);
            // If no quantity is provided, return empty string
            if (request.empty())
                return "";
            // Extract the quantity from the request
            string quantityStr = request.substr(0);
            int quantity = stoi(quantityStr);
            cmdMenu.addCommand(new AddToCartCommand(productId, products, quantity, user_id, clientCarts));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is adding a product to the cart");
            response = cmdMenu.executeCommand();
            saveProductsToFile(products, productsFile);
        }
        else if (request == "3") {
            cmdMenu.addCommand(new ViewCartCommand(user_id, clientCarts));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is viewing the cart");
            response = cmdMenu.executeCommand();
        }
        else if (request == "4") {
            // Request the product ID from the client
            int productId = productIdRequest(client_id, user_id);
            // If no product ID is provided, return empty string
            if (productId == 0)
                return "";
            cmdMenu.addCommand(new DeleteFromCartCommand(products, productId, user_id, clientCarts));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is deleting a product from the cart");
            response = cmdMenu.executeCommand();
            saveProductsToFile(products, productsFile);
        }
        else if (request == "5") {
            cmdMenu.addCommand(new ApplyDiscountCommand(user_id, clientCarts));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is appling discount to the cart");
            response = cmdMenu.executeCommand();
        }
        else if (request == "6") {
            cmdMenu.addCommand(new CheckoutCommand(user_id, clientCarts));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is checking out the cart");
            response = cmdMenu.executeCommand();
            // If the cart is empty, return the response no need to proceed with payment process
            if (response.find("empty") != string::npos) 
                return response;
            // Ask the client for the payment method
            response += "\nEnter payment method('C' for Card / 'U' for UPI): \n";
            sendresponse(client_id, response);
            request = recvrequest(client_id, user_id);
            // If no payment method is provided, return empty string
            if (request.empty())
                return "";
            // If an invalid input is provided, inform the client
            else if (request.length() != 1) {
                response = "Invalid input. Please enter a character ('c' / 'u')\n";
                LOG_INFO("Client " + Users[user_id].getUsername() + " entered invalid input");
                return response;
            }
            // Extract the payment choice from the request
            char paymentChoice = request.substr(0)[0];
            CheckoutCommand checkoutcmd;
            // Get the total amount from checkout command for the payment
            double totalAmount = checkoutcmd.gettotalAmount();
            cmdMenu.addCommand(new PaymentCommand(nullptr, paymentChoice, totalAmount));
            LOG_INFO("Client " + Users[user_id].getUsername() + " is processing the payment");
            response = cmdMenu.executeCommand();
            saveProductsToFile(products, productsFile);
            // If the payment is successful, clear the client's cart
            if (response.find("successful") != string::npos) {
                saveClientData(user_id);
                clientCarts.erase(user_id);
            }
        }
        else if (request == "7") {
            // Perform a cart check before logging out
            cartCheck(user_id);
            response = "Logging out successfully............";
            clientLoginStatus[user_id] = false;    // Login status is updated to false after logout
            LOG_INFO("Client " + Users[user_id].getUsername() + " is logging out\n");
        }
        else if (request == "8") {
            // Perform a cart check before exiting
            cartCheck(user_id);
            response = "\nThank you for visiting!!!\n";
            LOG_INFO("Client " + Users[user_id].getUsername() + " is exiting");
        }
        else {
            response = "Invalid choice. Please enter between 1 to 8.\n";
            LOG_INFO("Client " + Users[user_id].getUsername() + " entered invalid choice");
        }
    }
    catch (const exception& e) {
        LOG_ERROR("Exception caught in handling request: " + string(e.what()));
        response = "An error occurred while processing your request.";
    }
    return response;
}

// Receives data from client socket and send response back
void Server::handleClient(int client_id) {
    int user_id = 0;
    while (true) {

        string request = recvrequest(client_id, user_id);

        if (request.empty()) {
            break;
        }

        string response, username, password;
        // Assuming the request is in the format "command:data"
        size_t pos = request.find(':');
        if (pos != string::npos) {
            string command = request.substr(0, pos);
            string data = request.substr(pos + 1);
            size_t sep = data.find(',');
            username = data.substr(0, sep);
            password = data.substr(sep + 1);
           
            if (command == "register") {
                LOG_INFO("User is trying to signing up for first time");
                User newUser(0, username, password);
                newUser.initializeUserId();              
                if (newUser.createAccount()) {
                    response = "Registration successful";
                    user_id = User::lastUserId;
                    Users[user_id] = newUser;
                    registerNewClient(user_id);
                    LOG_INFO("Client " + to_string(user_id) + " is registered with username " + newUser.getUsername());
                }
                else {
                    response = "Username already exists";
                    LOG_INFO("Username " + newUser.getUsername() + " is already registered");
                }
                clientLoginStatus[user_id] = false;   // Add login status of client
            }
            else if (command == "login") {
                LOG_INFO("Client " + username + " is trying to log in");
                User user(0, username, password);
                if (user.loginAuth()) {
                    for (auto& pair : Users) {
                        if (pair.second.getUsername() == username) {
                            user_id = pair.first;
                        }
                    }
                    clientLoginStatus[user_id] = true;   // Add login status of client
                    response = "Authentication successful";
                    LOG_INFO("Client " + user.getUsername() + " has successfully logged in");
                }
                else {
                    response = "Invalid username or password";
                    LOG_INFO("Client " + user.getUsername() + " has entered invalid username or password");
                }
            }
        }
        else {
            response = handleRequest(request, client_id, user_id);
        }

        if (response.empty()) {
            break;
        }

        sendresponse(client_id, response);

        // If the client sends "8", it's a signal to close the connection
        if (request == "8") {
            LOG_INFO("Closing connection for client " + Users[user_id].getUsername() + "\n");
            break;
        }
    }
    // Close the client socket as the interaction is complete
    closesocket(client_id);

    clientLoginStatus.erase(user_id);
}


void Server::registerNewClient(int user_id) {
    ofstream file("client_data/" + Users[user_id].getUsername() + ".csv");
    // Initially, the file can be empty or contain some default headers.
    file << "Product_ID,Category,Product_Name,Price,Quantity\n";
}

void Server::saveClientData(int user_id) {
    ofstream file("client_data/" + Users[user_id].getUsername() + ".csv");

    if (file.is_open()) {
        for (Product* product : clientCarts[user_id]) {
            file << product->getProdId() << ','
                << product->getCategory() << ','
                << product->getName() << ','
                << product->getPrice() << ','
                << product->getQuantity() << '\n';
        }
    }
}