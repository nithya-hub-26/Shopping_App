#include "Logger.h"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const string& message) {
    lock_guard<mutex> lock(mtx);
    string levelStr;
    cout << message << endl;
    switch (level) {
    case LogLevel::INFO: levelStr = "[INFO] "; break;
    case LogLevel::WARN: levelStr = "[WARN] "; break;
    case LogLevel::ERR: levelStr = "[ERROR] "; break;
    }
    string msginfo = levelStr + getCurrentTime() + "Thread ID: ";
    logfile << msginfo << this_thread::get_id() << " " << message << endl;
}

Logger::Logger() {
    logfile.open("logfile1.log", ios::out | ios::app);
    if (!logfile.is_open()) {
        throw runtime_error("Unable to open log file");
    }
}

string Logger::getCurrentTime() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", localtm);
    return string(buffer);
}

Logger::~Logger() {
    if (logfile.is_open()) {
        logfile.close();
    }
}