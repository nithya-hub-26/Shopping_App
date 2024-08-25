#include "User.h"

mutex userMutex;
static const string FILE_NAME = "clients.csv";
int User::lastUserId = 0;

User::User(): userId(0) {}

User::User(int userId,string username, string password)
    : userId(userId), username(username), password(password) {}

int User::getuserId() {
    return userId;
}

string User::getUsername() {
    return username;
}

void User::initializeUserId() {
    ifstream infile(FILE_NAME);
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string stored_userId;
        getline(ss, stored_userId, ',');
        int userid = stoi(stored_userId);
        if (userid > lastUserId) {
            lastUserId = userid;
        }
    }
}

bool User::createAccount() {
    return registerUser(username, password, userId);
}

bool User::loginAuth() {
    return authenticateUser(username, password);
}

bool User::registerUser(string& username, string& password, int& userId) {
    lock_guard<mutex> lock(userMutex);

    ifstream infile(FILE_NAME);
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string stored_username, stored_password;
        getline(ss, stored_username, ',');
        if (stored_username == username) {
            return false; // Username already exists
        }
    }
    userId = ++lastUserId;
    ofstream outfile(FILE_NAME, ios::app);
    outfile << userId << "," << username << "," << password << "\n";
    return true;
}

bool User::authenticateUser(string& username, string& password) {
    lock_guard<mutex> lock(userMutex);

    ifstream infile(FILE_NAME);
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string stored_userId, stored_username, stored_password;
        getline(ss, stored_userId, ',');
        getline(ss, stored_username, ',');
        getline(ss, stored_password, ',');
        if (stored_username == username && stored_password == password) {
            return true; // Authentication successful
        }
    }
    return false; // Authentication failed
}