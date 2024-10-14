#include "libraries.h"
#include <ctime>
#include <time.h>

// int main() {
//     srand(time(NULL));
//     int size;
//     char input;

//     std::cout << "Welcome to the game!" << std::endl;
//     std::cout << "Please select a grid size:" << std::endl;
//     std::cout << "1. 10x10" << std::endl;
//     std::cout << "2. 15x15" << std::endl;
//     std::cout << "3. 20x20" << std::endl;
//     std::cout << "Enter your choice (1/2/3): ";
//     std::cin >> input;

//     switch (input) {
//         case '1':
//             size = 10;
//             break;
//         case '2':
//             size = 15;
//             break;
//         case '3':
//             size = 20;
//             break;
//         default:
//             std::cout << "Invalid choice. Defaulting to 15x15 grid." << std::endl;
//             size = 10;
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

    time_t startTime = time(NULL);
    time_t currTime;
    const double interval = 10.0;

    Game game(15);
    game.print();
    game.showGrid();

    while (game.getPlayerMoves())
    {
        currTime = time(NULL);
        if (difftime(currTime, startTime) >= interval) {
            game.updateCoinsLocation();
        }
        
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