#include "CommandMenu.h"

CommandMenu::~CommandMenu() {
    clearCommands();
}
void CommandMenu::addCommand(Command* command) {
    commands.push_back(command);
}

string CommandMenu::executeCommand() {
    string response;
    response = commands[commands.size() - 1]->execute();
    return response;
}

void CommandMenu::clearCommands() {
    for (auto command : commands) {
        delete command;
    }
    commands.clear();
}