#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
using namespace std;

class User {
    int userId;
    string username;
    string password;

public:
    User();
    User(int userId, string username, string password);

    int getuserId();
    string getUsername();
    void initializeUserId();
    bool createAccount();
    bool loginAuth();
    static int lastUserId;

    static bool registerUser(string& username, string& password, int& userId);
    static bool authenticateUser(string& username, string& password);
};

#endif // USER_H