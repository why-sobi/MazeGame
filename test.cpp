#include "libraries.h"
#include <ctime>

// int main() {
//     srand(time(NULL));
//     int size;
//     char input;

//     std::cout << "Welcome to the game!" << std::endl;
//     std::cout << "Please select a grid size:" << std::endl;
//     std::cout << "1. 15x15" << std::endl;
//     std::cout << "2. 20x20" << std::endl;
//     std::cout << "3. 25x25" << std::endl;
//     std::cout << "Enter your choice (1/2/3): ";
//     std::cin >> input;

//     switch (input) {
//         case '1':
//             size = 15;
//             break;
//         case '2':
//             size = 20;
//             break;
//         case '3':
//             size = 25;
//             break;
//         default:
//             std::cout << "Invalid choice. Defaulting to 15x15 grid." << std::endl;
//             size = 15;
//     }

//     Game game(size);
//     game.print();
//     game.showGrid();

//     while (game.getPlayerMoves()) {
//         std::cout << "Enter your move (W/A/S/D) or 'U' to undo: ";
//         std::cin >> input;
//         if (input == 'u' || input == 'U')
//             game.undoMove();
//         else
//             game.movePlayer(input);
//     }

//     std::cout << "Congratulations! You've reached the end of the game." << std::endl;
//     return 0;
// }

int main()
{
    srand(time(NULL));

    Game game(15);
    game.print();
    game.showGrid();

    while (game.getPlayerMoves())
    {
        char input;
        std::cin >> input;
        if (input == 'u' || input == 'U')
            game.undoMove();
        else
        {
            game.movePlayer(input);
        }
    }
    return 0;
}