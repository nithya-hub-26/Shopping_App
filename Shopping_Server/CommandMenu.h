#ifndef COMMAND_MENU_H
#define COMMAND_MENU_H

#include "Command.h"

class CommandMenu {
private:
    vector<Command*> commands;

public:
    ~CommandMenu();
    void addCommand(Command* command);
    string executeCommand();
    void clearCommands();
};

#endif