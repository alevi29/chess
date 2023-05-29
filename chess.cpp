#include <iostream>
#include "chess.h"

bool gameOver = false;


int main() {
    gameloop:

    boardSetup(theBoard);
    showBoard(theBoard);
    gameOver = false;

    while (!gameOver) {
        move(white, theBoard);
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
