#include <iostream>
#include <random>
#include "libraries.h"

int main()
{
    srand(time(NULL));

    int size;
    char input;

    std::cout << "Welcome to the game!" << std::endl;
    std::cout << "Please select a grid size:" << std::endl;
    std::cout << "1. 10x10" << std::endl;
    std::cout << "2. 15x15" << std::endl;
    std::cout << "3. 20x20" << std::endl;
    std::cout << "Enter your choice (1/2/3): ";
    std::cin >> input;

    switch (input)
    {
    case '1':
        size = 10;
        break;
    case '2':
        size = 15;
        break;
    case '3':
        size = 20;
        break;
    default:
        std::cout << "Invalid choice. Defaulting to 15x15 grid." << std::endl;
        size = 15;
    }

    Game game(size);
    game.startGame();

    return 0;
}
