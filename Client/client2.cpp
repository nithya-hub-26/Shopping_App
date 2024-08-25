#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

void handleUserInput(int client_fd);
void userAuthentication(int client_id);

// Function to send a request to the server
void sendrequest(int client_fd, string request) {
    if (send(client_fd, request.c_str(), request.length(), 0) == SOCKET_ERROR) {
        cerr << "Send failed" << endl;
        closesocket(client_fd);
        WSACleanup();
        exit(1);
    }
}

// Function to receive a response from the server
string recvresponse(int client_fd) {
    // Receive the response from the server
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    // If no bytes are read, the server has disconnected
    if (bytesRead <= 0) {
        cerr << "Server disconnected" << endl;
        closesocket(client_fd);
        WSACleanup();
        exit(1);
    }

    buffer[bytesRead] = '\0';
    string request = buffer;

    return request;
}

// Function to listen for UDP broadcasts on a specified port
void listenForBroadcast(int udpPort) {
    // Create a UDP socket
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == INVALID_SOCKET) {
        cerr << "UDP socket creation failed" << endl;
        return;
    }

    // Define the socket address structure for the UDP socket
    sockaddr_in udpAddr;
    udpAddr.sin_family = AF_INET;   // Set the family to IPv4
    udpAddr.sin_port = htons(udpPort);    // Convert the port number to network byte order
    udpAddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces

    // Bind the socket to the specified port
    if (bind(udpSocket, (sockaddr*)&udpAddr, sizeof(udpAddr)) < 0) {
        cerr << "UDP socket bind failed" << endl;
        closesocket(udpSocket);
        return;
    }

    // Buffer to store the incoming broadcast message
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));   // Clear the buffer
        // Receive a broadcast message
        int len = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
        if (len > 0) {
            buffer[len] = '\0';   // Null-terminate the received string
            // Print the received broadcast message
            cout << "\n\n\t/**** Broadcast message: " << buffer << "****/\n" << endl;
        }
    }
    closesocket(udpSocket);
}

// Function to validate user input and ensure it's an integer
int getValidatedInt(string& prompt) {
    int number, c;
    while (true) {
        cout << prompt;
        cin >> number;

        // Check if the input is valid
        if (cin.fail()) {
            cin.clear(); // Clear the error state
            while ((c = cin.get()) != '\n') {} // Clears invalid input
            cout << "Invalid input. Please enter an integer.\n";
            continue;
        }
        return number;
    }
}

// Function to set the product ID based on server response
string setProductId(int client_fd) {
    string response = recvresponse(client_fd);
    // Get the validated product id
    int prodId = getValidatedInt(response);;
    string request = to_string(prodId);
    return request;
}

bool userAuth(int client_id) {
    system("cls");
    cout << "\n\n\t\t\t SHOPPING APPLICATION\n\n";

    cout << "\n\t\t 1. Login\n\t\t 2. SignUp\n\n\t\t Enter your choice: ";
    int choice;
    while (!(cin >> choice)) {   // Error handling for string input
        cout << "\n\t\tInvalid input. Please enter a number: ";
        cin.clear();
        while (cin.get() != '\n');
    }
    string request, uname, upass;
    system("cls");
    if (choice == 1) {
        cout << "\t\t\tLOGIN";
        cout << "\n\n\t\tUsername: ";
        cin >> uname;
        cout << "\n\n\t\tPassword: ";
        cin >> upass;
        request = "login:" + uname + "," + upass;
    }
    else if (choice == 2) {
        cout << "              CREATE ACCOUNT                  \n";
        cout << "\nEnter the username: ";
        cin >> uname;
        cout << "\nEnter the password: ";
        cin >> upass;
        request = "register:" + uname + "," + upass;
    }
    else {
        system("cls");
        cout << "\n\nInvalid Choice\n\n" << endl;
        system("pause");
        userAuth(client_id);
    }
    sendrequest(client_id, request);
    string response = recvresponse(client_id);
    cout << "Server response: \n\n" << response << endl;
    cout << "\n";
    system("pause");
    if (request.find("login") != string::npos && response.find("success") != string::npos) {
        return true;
    }
    return false;
}

// Function to handle user input and interact with the server
void handleUserInput(int client_fd) {
    int choice;
    while (true) {
        bool serveresponseFlag = false;   // Flag to check server response validity
        string request = "0";             // Default request value
        try {
            sendrequest(client_fd, request);
            string response = recvresponse(client_fd);
            choice = getValidatedInt(response);
            switch (choice) {
            case 1:
                request = "1";
                break;
            case 2: {
                request = "2";
                sendrequest(client_fd, request);
                request = setProductId(client_fd);
                sendrequest(client_fd, request);
                response = recvresponse(client_fd);
                if (response.find("Invalid") != string::npos) {
                    serveresponseFlag = true;    // Set flag if response is invalid
                    break;
                }
                int quantity = getValidatedInt(response);
                request = to_string(quantity);
                break;
            }
            case 3:
                request = "3";
                break;
            case 4: {
                request = "4";
                sendrequest(client_fd, request);
                request = setProductId(client_fd);
                break;
            }
            case 5:
                request = "5";
                break;
            case 6: {
                request = "6";
                sendrequest(client_fd, request);
                response = recvresponse(client_fd);
                if (response.find("empty") != string::npos) {
                    serveresponseFlag = true;    // Set flag if response is "empty"
                    break;
                }
                cout << response;
                string paymentChoice;
                int c;
                // Clear the buffer until a new line occurs
                while ((c = cin.get()) != '\n') {}
                getline(cin, paymentChoice);
                request = paymentChoice;
                break;
            }
            case 7:
                request = "7";
                break;
            case 8:
                request = "8";
                break;
            default:
                request = to_string(choice);
            }

            if (serveresponseFlag) {
                cout << "Server response: \n\n" << response << endl;
            }
            else {
                sendrequest(client_fd, request);
                string buffer = recvresponse(client_fd);
                cout << "Server response: \n\n" << buffer << endl;

                if (buffer.find("Logging out") != string::npos) {
                    Sleep(4000);
                    userAuthentication(client_fd);
                    break;
                }

                if (buffer.find("visiting") != string::npos) {
                    cout << "Exiting....." << endl;
                    Sleep(4000);    // Wait for 4 seconds before exiting
                    break;
                }
            }
        }
        catch (const exception& e) {
            cerr << "Exception caught in user input: " << e.what() << endl;
            cout << "An error occured. Exiting...\n";
            break;
        }
    }
}

void userAuthentication(int client_id) {
    while (true) {
        bool handleUserFlag = userAuth(client_id);
        if (handleUserFlag) {
            system("cls");
            // Handle user input and server interaction
            handleUserInput(client_id);
            break;
        }
    }
}

int main() {
    // Initializes the Winsock library
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << endl;
        return 1;
    }

    // Client socket creation
    SOCKET client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // Define the server address structure
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    // Converts the IP address to sockaddr_in address structure
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        cerr << "Connection failed" << endl;
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }

    // Start a thread to listen for UDP broadcasts
    thread udpListenerThread(listenForBroadcast, 8081);
   
    userAuthentication(client_fd);

    // Clean up resources before exiting
    udpListenerThread.detach();
    closesocket(client_fd);
    WSACleanup();
    return 0;
}