#include <iostream>
#include "StudentManager.h"

int main()
{
    StudentManager manager;
    MyParser parser;
    std::string user_command;
    bool running{true};
    while (running)
    {
        std::getline(std::cin, user_command);
        running = manager.process(parser(user_command));
    }

    return 0;
}