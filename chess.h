#ifndef CHESS_CHESS_H
#define CHESS_CHESS_H

#include <stack>
#include <string>
#include <cmath>

#define BCONT 0
#define WCONT 1
#define BWCONT 2
#define FREE 3

// definitions for control board

// *** create control board ***
// *** special cases for king moves (move into check, castle) ***
// *** implement piece taking ***
// *** pawn promotion ***

/*
relative piece strengths:
    pawn: 1
    knight: 3
    bishop: 3.5
    rook: 5
    queen: 9
*/

struct piece {
    char ID;
    std::string color;
    bool canDoubleMove;
};

std::stack<std::string> moves; // used to check for three-fold repetition
piece theBoard[8][8], cBoard[8][8];

struct player {
    int pCount = 8; // number of remaining pawns
    int strength = 40; // strength of remaining pieces
    int KX, KY; // location of king
    bool canCastle = true; // ability to castle
    std::string color;
};

player white, black;

void boardSetup(piece board[][8]) {
    white.color= "White";
    white.KX = 3;
    white.KY = 7;
    black.color = "Black";
    black.KX = 3;
    black.KY = 0;
    for (int i = 0; i < 8; ++ i) {
        for (int j = 0; j < 8; ++j) {
            if (i < 2) board[i][j].color = "Black";
            else if (i > 5) board[i][j].color = "White";

            if (i != 1 && i != 6) {
                board[i][j].ID = ' ';
                board[i][j].canDoubleMove = false;
            }
            else {
                board[i][j].ID = 'P';
                board[i][j].canDoubleMove = true;
            }
        }
    }
    board[0][0].ID = 'R';
    board[0][7].ID = 'R';
    board[7][0].ID = 'R';
    board[7][7].ID = 'R';

    board[0][1].ID = 'n';
    board[0][6].ID = 'n';
    board[7][1].ID = 'n';
    board[7][6].ID = 'n';

    board[0][2].ID = 'B';
    board[0][5].ID = 'B';
    board[7][2].ID = 'B';
    board[7][5].ID = 'B';

    board[0][3].ID = 'Q';
    board[0][4].ID = 'K';
    board[7][3].ID = 'Q';
    board[7][4].ID = 'K';
}

void showBoard(piece board[][8]) {
    std::cout << "---------------------------------\n" <<
              "| " << board[0][0].ID << " | " << board[0][1].ID << " | " << board[0][2].ID <<
              " | " << board[0][3].ID << " | " << board[0][4].ID << " | " << board[0][5].ID <<
              " | " << board[0][6].ID << " | " << board[0][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[1][0].ID << " | " << board[1][1].ID << " | " << board[1][2].ID <<
              " | " << board[1][3].ID << " | " << board[1][4].ID << " | " << board[1][5].ID <<
              " | " << board[1][6].ID << " | " << board[1][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[2][0].ID << " | " << board[2][1].ID << " | " << board[2][2].ID <<
              " | " << board[2][3].ID << " | " << board[2][4].ID << " | " << board[2][5].ID <<
              " | " << board[2][6].ID << " | " << board[2][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[3][0].ID << " | " << board[3][1].ID << " | " << board[3][2].ID <<
              " | " << board[3][3].ID << " | " << board[3][4].ID << " | " << board[3][5].ID <<
              " | " << board[3][6].ID << " | " << board[3][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[4][0].ID << " | " << board[4][1].ID << " | " << board[4][2].ID <<
              " | " << board[4][3].ID << " | " << board[4][4].ID << " | " << board[4][5].ID <<
              " | " << board[4][6].ID << " | " << board[4][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[5][0].ID << " | " << board[5][1].ID << " | " << board[5][2].ID <<
              " | " << board[5][3].ID << " | " << board[5][4].ID << " | " << board[5][5].ID <<
              " | " << board[5][6].ID << " | " << board[5][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[6][0].ID << " | " << board[6][1].ID << " | " << board[6][2].ID <<
              " | " << board[6][3].ID << " | " << board[6][4].ID << " | " << board[6][5].ID <<
              " | " << board[6][6].ID << " | " << board[6][7].ID << " |\n" <<
              "---------------------------------\n" <<
              "| " << board[7][0].ID << " | " << board[7][1].ID << " | " << board[7][2].ID <<
              " | " << board[7][3].ID << " | " << board[7][4].ID << " | " << board[7][5].ID <<
              " | " << board[7][6].ID << " | " << board[7][7].ID << " |\n" <<
              "---------------------------------\n"
              << std::endl;
}

bool validPiece(const std::string& piece, const player& current, struct piece board[][8]) {
    if (piece.length() != 3) return false;
    if (piece[1] > 'H' || piece[1] < 'A') return false;
    if (piece[2] > '8' || piece[2] < '1') return false;
    // if given coordinates are invalid

    if (piece[0] != board[abs(piece[2] - 56)][piece[1] - 65].ID || board[abs(piece[2] - 56)][piece[1] - 65].color != current.color) return false;
    // if target location does not have specified piece/ piece belongs to opposite color

    return true;
}

bool validMove(const std::string& move, const player& current, struct piece board[][8], int curRow, int curCol) {
    if (move.length() != 2) return false;
    if (move[0] > 'H' || move[0] < 'A') return false;
    if (move[1] > '8' || move[1] < '1') return false;
    // if given coordinates are invalid

    int targRow = abs(move[1] - 56), targCol = move[0] - 65;

    if (board[targRow][targCol].ID != ' ' && board[targRow][targCol].color == current.color) return false;
    // if target location is occupied by a piece of the same color

    piece currentPiece = board[curRow][curCol];
    switch(currentPiece.ID) {
        case 'P':
            if ((current.color == "White" && (targCol == curCol + 1 || targCol == curCol - 1) && targRow == curRow - 1
                && board[targRow][targCol].ID != ' ') || (current.color == "Black" && (targCol == curCol + 1 || targCol == curCol - 1)
                && targRow == curRow + 1 && board[targRow][targCol].ID != ' ')) return true;
            // check for pawn captures

            if (currentPiece.canDoubleMove) {
                if (curCol == targCol && current.color == "White" ? targRow - curRow < 0 && curRow - targRow < 3
                : targCol - curCol < 0 && targRow - curRow < 3) {
                    if (current.color == "White" ? targRow - curRow == -2 : targRow - curRow == 2) currentPiece.canDoubleMove = false;
                    return true;
                }
                return false;
            }
            // check for pawn first move

            else {
                if ((targCol == curCol + 1 || targCol == curCol - 1) && move[0] == moves.top()[1] &&
                    current.color == "White" ? targRow == curRow - 1 && move[1] == moves.top()[4] + 1
                    : targRow == curRow + 1 && move[1] == moves.top()[4] - 1) {
                    if (moves.top()[0] == 'P' && current.color == "White" ? moves.top()[2] == '7' && moves.top()[4] == '5'
                        : moves.top()[2] == '2' && moves.top()[4] == '4') return true;
                    return false;
                }
                // check for en passant

                return targCol == curCol && current.color == "White" ? targRow - curRow < 0 && curRow - targRow < 2
                : curRow - targRow < 0 && targRow - curRow < 2;
            }
            // check for all other pawn moves

        case 'R':
            if (curRow != targRow && curCol != targCol) return false;
            // illegal rook movement (mismatching rank and file)

            if (targCol == curCol) {
                for (int i = curRow < targRow ? curRow : targRow; i < curRow < targRow ? targRow : curRow; ++i) {
                    if (board[i][targCol].ID != ' ') return false;
                }
                return true;
            }
            // check for movement along a file

            else {
                for (int i = curCol < targCol ? curCol : targCol; i < curCol < targCol ? targCol : curCol; ++i) {
                    if (board[curRow][i].ID != ' ') return false;
                }
                return true;
            }
            // check for movement along a rank

        case 'n':
            if ((targRow == curRow + 1 || targRow == curRow - 1) && (targCol == curCol + 2 || targCol == curCol - 2)) return true;
            if ((targRow == curRow + 2 || targRow == curRow - 2) && (targCol == curCol + 1 || targCol == curCol - 1)) return true;
            return false;

        case 'B':
        case 'K':
        case 'Q':
            break;
    }
    return true;
}

std::string checkPiece(const char& curPiece) {
    std::string piece;
    switch(curPiece) {
        case 'P':
            piece = "pawn";
            break;
        case 'R':
            piece = "rook";
            break;
        case 'n':
            piece = "knight";
            break;
        case 'B':
            piece = "bishop";
            break;
        case 'Q':
            piece = "queen";
            break;
        case 'K':
            piece = "king";
            break;
        default:
            break;
    }
    return piece;
}

bool canMove(int row, int col) {
    return true;
}

void move(player &current, piece board[][8]) {
    std::string inputPiece, inputMove;
    std::cout << current.color << "'s turn to move. " <<
    "Please specify the piece you wish to move and it's location (PE1 for pawn E1).\n";

    pieceSelect:
    std::cin >> inputPiece;
    if (!validPiece(inputPiece, current, board)) {
        std::cout << "Invalid input/piece is unable to move, please input the piece you wish to move again.\n";
        goto pieceSelect;
    }
    int curRow = abs(inputPiece[2] - 56), curCol = inputPiece[1] - 65;
    std::cout << "Please specify where you would like to move your " << checkPiece(inputPiece[0]) << " (D4 to move to D4).\n";

    pieceMove:
    std::cin >> inputMove;
    if (!validMove(inputMove, current, board, curRow, curCol)) {
        std::cout << "Invalid input/piece can not move there, please input where you would like to move again.\n";
        goto pieceMove;
    }
    int targRow = abs(inputMove[1] - 56), targCol = inputMove[0] - 65;

    std::string move = inputPiece + inputMove;
    moves.push(move); // add current move to the stack

    if (board[curRow][curCol].ID == 'P') board[curRow][curCol].canDoubleMove = false; // if pawn was moved, disable
    if (board[curRow][curCol].ID == 'K') {
        current.KX = targCol;
        current.KY = targRow;
    } // if king was moved, update king coordinates

    if (board[targRow][targCol].ID != ' ') {
        switch (board[targRow][targCol].ID) {
            case 'P':
                if (current.color == "White") {
                    black.pCount--;
                    black.strength--;
                }
                else {
                    white.pCount--;
                    white.strength--;
                }
                break;
            case 'R':
                current.color == "White" ? black.strength -= 5 : white.strength -= 5;
            case 'Q':
                current.color == "White" ? black.strength -= 9 : white.strength -= 9;
                break;
            default:
                current.color == "White" ? black.strength -= 3 : white.strength -= 3;
        }
    }
    // check if a piece was taken
}

bool checkStalemate(const player& current, piece board[][8]) {
    if (current.strength == 0) return true;
    // player only has king left

    if (current.pCount == 0 && (current.strength == 3 || current.strength == 3.5 || current.strength == 6)) return true;
    // player only has king + knight / king + bishop + king + 2 knights

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].color == current.color && board[i][j].ID != ' ' && canMove(i, j)) {
                return false;
            }
        }
    }
    return true;
}

#endif //CHESS_CHESS_H
