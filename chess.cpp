#include <iostream>
#include "chess.h"

int main() {
    gameloop:

    boardSetup();
    showBoard();
    gameOver = false;

    while (!gameOver) {
        move(white);
    }
/*
    std::cout << "Do you wish to play again? Y/N";
    std::string response;
    std::cin >> response;
    if (response == "Y") goto gameloop;
    else {

    }
    */
    return 0;
}
