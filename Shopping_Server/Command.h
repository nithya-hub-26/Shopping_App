#ifndef COMMAND_H
#define COMMAND_H

//#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Product.h"
#include "Logger.h"
using namespace std;

class Command {
public:
    virtual ~Command() {}
    virtual string execute() = 0;
};

#endif
