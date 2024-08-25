#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>
#include <ctime>
#include <thread>
using namespace std;

class Logger {
public:
    enum class LogLevel {
        INFO,
        WARN,
        ERR
    };

    static Logger& getInstance();
    void log(LogLevel level, const string& message);

private:
    Logger();
    string getCurrentTime();
    ~Logger();

    ofstream logfile;
    mutex mtx;
};

#define LOG_INFO(msg) Logger::getInstance().log(Logger::LogLevel::INFO, msg)
#define LOG_WARN(msg) Logger::getInstance().log(Logger::LogLevel::WARN, msg)
#define LOG_ERROR(msg) Logger::getInstance().log(Logger::LogLevel::ERR, msg)

#endif 