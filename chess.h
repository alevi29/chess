#ifndef CHESS_CHESS_H
#define CHESS_CHESS_H

#include <stack>
#include <string>
#include <cmath>
#include <unordered_map>
#include "ConsoleColor.h"

#define BCONT 0
#define WCONT 1
#define BWCONT 2
#define FREE 3
#define TXT (board[i][j].color == "White" ? whiteColor : green)
#define WC whiteColor
#define cBoardCheck if (color == "White") { \
if (curCBoard[cRow][cCol] == FREE) curCBoard[cRow][cCol] = WCONT; \
else if (curCBoard[cRow][cCol] == BCONT) curCBoard[cRow][cCol] = BWCONT; \
}                                           \
else {                                      \
if (curCBoard[cRow][cCol] == FREE) curCBoard[cRow][cCol] = BCONT; \
else if (curCBoard[cRow][cCol] == WCONT) curCBoard[cRow][cCol] = BWCONT; \
}                                           \
if (curBoard[bRow][bCol].ID == 'K') curBoard[i][j].isAttackingKing = true;

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
    std::string color = " ";
    bool canDoubleMove, isAttackingKing = false;
};

std::stack<std::string> moves;
std::unordered_map <std::string , int> moves2;
std::string boardState;
piece board[8][8], simBoard[8][8];
int cBoard[12][12], cSimBoard[12][12];
bool padBoard[12][12];
bool gameOver = false;

struct player {
    int pCount = 8; // number of remaining pawns
    int strength = 40; // strength of remaining pieces
    int KX, KY; // location of king
    bool leftCastle = true, rightCastle = true; // ability to castle
    std::string color;
};

player white, black;

bool validPiece(const std::string& piece, const player& current) {
    if (piece.length() != 3) return false;
    if (piece[1] > 'H' || piece[1] < 'A') return false;
    if (piece[2] > '8' || piece[2] < '1') return false;
    // if given coordinates are invalid

    if (piece[0] != board[abs(piece[2] - 56)][piece[1] - 65].ID || board[abs(piece[2] - 56)][piece[1] - 65].color != current.color) return false;
    // if target location does not have specified piece/ piece belongs to opposite color

    return true;
}

bool canMove(int row, int col, const player& current) {
    switch (board[row][col].ID) {
        case 'P':
            if (current.color == "White") {
                if (board[row - 1][col].ID == ' ') return true;
                if (col == 7) {
                    return board[row - 1][col - 1].ID != ' ' && board[row - 1][col - 1].color != current.color;
                }
                else if (col == 0) {
                    return board[row - 1][col + 1].ID != ' ' && board[row - 1][col + 1].color != current.color;
                }
                else {
                    return board[row - 1][col - 1].ID != ' ' && board[row - 1][col - 1].color != current.color
                           || board[row - 1][col + 1].ID != ' ' && board[row - 1][col + 1].color != current.color;
                }
            }
            else {
                if (board[row + 1][col].ID == ' ') return true;
                if (col == 7) {
                    return board[row + 1][col - 1].ID != ' ' && board[row - 1][col - 1].color != current.color;
                }
                else if (col == 0) {
                    return board[row + 1][col + 1].ID != ' ' && board[row - 1][col + 1].color != current.color;
                }
                else {
                    return board[row + 1][col - 1].ID != ' ' && board[row - 1][col - 1].color != current.color
                           || board[row + 1][col + 1].ID != ' ' && board[row - 1][col + 1].color != current.color;
                }
            }
        case 'R':
            return !padBoard[row + 2][col + 3] || !padBoard[row + 2][col + 1] || !padBoard[row + 3][col + 2] || !padBoard[row + 1][col + 2];
        case 'n':
            return !padBoard[row + 3][col + 4] || !padBoard[row + 3][col] ||
                   !padBoard[row + 4][col + 3] || !padBoard[row + 4][col + 1] ||
                   !padBoard[row + 1][col + 4] || !padBoard[row + 1][col] ||
                   !padBoard[row][col + 3] || !padBoard[row][col + 1];
        case 'B':
            return !padBoard[row + 3][col + 3] || !padBoard[row + 3][col + 1]
                   || !padBoard[row + 1][col + 3] || !padBoard[row + 1][col + 1];
        case 'Q':
            return !padBoard[row + 3][col + 3] || !padBoard[row + 3][col + 1] ||
                   !padBoard[row + 1][col + 3] || !padBoard[row + 1][col + 1] ||
                   !padBoard[row + 2][col + 3] || !padBoard[row + 2][col + 1] ||
                   !padBoard[row + 3][col + 2] || !padBoard[row + 1][col + 2];
        default:
            if (current.color == "White") {
                return (!padBoard[row + 3][col + 3] && (cBoard[row + 3][col + 3] == WCONT || cBoard[row + 3][col + 3] == FREE)
                        || (!padBoard[row + 3][col + 1] && cBoard[row + 3][col + 1] == WCONT || cBoard[row + 3][col + 1] == FREE)
                        || (!padBoard[row + 1][col + 3] && cBoard[row + 1][col + 3] == WCONT || cBoard[row + 1][col + 3] == FREE)
                        || (!padBoard[row + 1][col + 1] && cBoard[row + 1][col + 1] == WCONT || cBoard[row + 1][col + 1] == FREE)
                        || (!padBoard[row + 2][col + 3] && cBoard[row + 2][col + 3] == WCONT || cBoard[row + 2][col + 3] == FREE)
                        || (!padBoard[row + 2][col + 1] && cBoard[row + 2][col + 1] == WCONT || cBoard[row + 2][col + 1] == FREE)
                        || (!padBoard[row + 3][col + 2] && cBoard[row + 3][col + 2] == WCONT || cBoard[row + 3][col + 2] == FREE)
                        || (!padBoard[row + 1][col + 2] && cBoard[row + 1][col + 2] == WCONT || cBoard[row + 1][col + 2] == FREE));
            }
            else {
                return (!padBoard[row + 3][col + 3] && (cBoard[row + 3][col + 3] == BCONT || cBoard[row + 3][col + 3] == FREE)
                        || (!padBoard[row + 3][col + 1] && cBoard[row + 3][col + 1] == BCONT || cBoard[row + 3][col + 1] == FREE)
                        || (!padBoard[row + 1][col + 3] && cBoard[row + 1][col + 3] == BCONT || cBoard[row + 1][col + 3] == FREE)
                        || (!padBoard[row + 1][col + 1] && cBoard[row + 1][col + 1] == BCONT || cBoard[row + 1][col + 1] == FREE)
                        || (!padBoard[row + 2][col + 3] && cBoard[row + 2][col + 3] == BCONT || cBoard[row + 2][col + 3] == FREE)
                        || (!padBoard[row + 2][col + 1] && cBoard[row + 2][col + 1] == BCONT || cBoard[row + 2][col + 1] == FREE)
                        || (!padBoard[row + 3][col + 2] && cBoard[row + 3][col + 2] == BCONT || cBoard[row + 3][col + 2] == FREE)
                        || (!padBoard[row + 1][col + 2] && cBoard[row + 1][col + 2] == BCONT || cBoard[row + 1][col + 2] == FREE));
            }
    }
}

bool validMove(const std::string &move, int curRow, int curCol) {
    player current = board[curRow][curCol].color == "White" ? white : black;

    if (move.length() != 2) return false;
    if (move[0] > 'H' || move[0] < 'A') return false;
    if (move[1] > '8' || move[1] < '1') return false;
    // if given coordinates are invalid

    int targRow = abs(move[1] - 56), targCol = move[0] - 65;

    if (board[targRow][targCol].ID != ' ' && board[targRow][targCol].color == current.color) return false;
    // if target location is occupied by a piece of the same color

    piece currentPiece = board[curRow][curCol];
    int testCol = curCol < targCol ? curCol : targCol;
    switch(currentPiece.ID) {
        case 'P':
            if ((current.color == "White" && (targCol == curCol + 1 || targCol == curCol - 1) && targRow == curRow - 1
                 && board[targRow][targCol].ID != ' ') || (current.color == "Black" && (targCol == curCol + 1 || targCol == curCol - 1)
                                                           && targRow == curRow + 1 && board[targRow][targCol].ID != ' ')) return true;
            // check for pawn captures

            if (currentPiece.canDoubleMove) {
                if (curCol == targCol && current.color == "White" ? targRow - curRow < 0 && curRow - targRow < 3
                                                                  : curRow - targRow < 0 && targRow - curRow < 3) {
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
                                                                          : moves.top()[2] == '2' && moves.top()[4] == '4') {
                        if (current.color == "White") {
                            board[targRow + 1][targCol].ID = ' ';
                            board[targRow + 1][targCol].color = " ";
                        }
                        else {
                            board[targRow - 1][targCol].ID = ' ';
                            board[targRow - 1][targCol].color = " ";
                        }
                        return true;
                    }
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
            if (abs(targRow - curRow) != abs(targCol - curCol)) return false;
            for (int testRow = curRow < targRow ? curRow : targRow; testRow < (curRow < targRow ? targRow : curRow); ++testRow) {
                std::cout << testRow << ", " << testCol << std::endl;
                if (board[testRow][testCol].ID != ' ') return false;
                testCol++;
            }
            return true;

        case 'K':
            if (current.leftCastle) {
                if (targRow == curRow && targCol == curCol - 2) {
                    if (board[curRow][curCol - 1].ID != ' ' || board[curRow][curCol - 2].ID != ' '
                        || board[curRow][curCol - 3].ID != ' ') return false;

                    board[curRow][curCol - 1] = board[curRow][curCol - 4];
                    board[curRow][curCol - 4].ID = ' ';
                    board[curRow][curCol - 4].color = " ";
                    board[curRow][curCol - 4].canDoubleMove = false;

                    simBoard[curRow][curCol - 1] = board[curRow][curCol - 1];
                    simBoard[curRow][curCol - 4] = board[curRow][curCol - 4];

                    padBoard[curRow + 1][curCol + 1] = true;
                    padBoard[curRow - 2][curCol - 2]= false;

                    return true;
                }
            }
            if (current.rightCastle) {
                if (targRow == curRow && targCol == curCol + 2)
                {
                    if (board[curRow][curCol + 1].ID != ' ' || board[curRow][curCol + 2].ID != ' ') return false;

                    board[curRow][curCol + 1] = board[curRow][curCol + 3];
                    board[curRow][curCol + 3].ID = ' ';
                    board[curRow][curCol + 3].color = " ";
                    board[curRow][curCol + 3].canDoubleMove = false;

                    simBoard[curRow][curCol + 1] = board[curRow][curCol + 1];
                    simBoard[curRow][curCol + 3] = board[curRow][curCol + 3];

                    padBoard[curRow + 5][curCol + 5] = false;
                    padBoard[curRow + 3][curCol + 3] = true;

                    return true;
                }
            }
            return abs(targRow - curRow) <= 1 && abs(targCol - curCol) <= 1;

        case 'Q':
            if (abs(targRow - curRow) == abs(targCol - curCol)) {
                for (int testRow = curRow < targRow ? curRow : targRow; testRow < curRow < targRow ? targRow : curRow; ++testRow) {
                    if (board[testRow][testCol].ID != ' ') return false;
                    testCol = curCol < targCol ? testCol + 1 : testCol - 1;
                }
            }
                // diagonal movement check

            else {
                if (curRow != targRow && curCol != targCol) return false;
                // non-horizontal/vertical movement

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
            }
            break;
    }
    return true;
}

void cBoardUpdate(int curCBoard[12][12], piece curBoard[8][8]) {
    for (int i = 2; i < 10; ++i) {
        for (int j = 2; j < 10; ++j) {
            if (curBoard[i - 2][j - 2].ID != ' ') {

                int bRow = i - 2, bCol = j - 2, cRow = i, cCol = j;
                std::string color = curBoard[i - 2][j - 2].color;
                char piece = curBoard[i - 2][j - 2].ID;

                switch (piece) {
                    case 'P':
                        if (color == "White") {
                            if (curCBoard[i - 1][j - 1] == FREE) curCBoard[i - 1][j - 1] = WCONT;
                            if (curCBoard[i - 1][j - 1] == BCONT) curCBoard[i - 1][j - 1] = BWCONT;
                            if (curCBoard[i - 1][j + 1] == FREE) curCBoard[i - 1][j + 1] = WCONT;
                            if (curCBoard[i - 1][j + 1] == BCONT) curCBoard[i - 1][j + 1] = BWCONT;
                            if (curBoard[i - 3][j - 1].ID == 'K') curBoard[i][j].isAttackingKing = true;
                            if (curBoard[i - 3][j - 1].ID == 'K') curBoard[i][j].isAttackingKing = true;
                        }
                        else {
                            if (curCBoard[i + 1][j - 1] == FREE) curCBoard[i - 1][j - 1] = BCONT;
                            if (curCBoard[i + 1][j - 1] == WCONT) curCBoard[i - 1][j - 1] = BWCONT;
                            if (curCBoard[i + 1][j + 1] == FREE) curCBoard[i - 1][j + 1] = BCONT;
                            if (curCBoard[i + 1][j + 1] == WCONT) curCBoard[i - 1][j + 1] = BWCONT;
                            if (curBoard[i - 1][j - 3].ID == 'K') curBoard[i][j].isAttackingKing = true;
                            if (curBoard[i - 1][j - 1].ID == 'K') curBoard[i][j].isAttackingKing = true;
                        }
                        break;

                    case 'R':
                        while (bCol < 7 && curBoard[bRow][++bCol].ID != ' ') {
                            cCol++;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;
                        // rook right movement

                        while (bCol > 0 && curBoard[bRow][--bCol].ID != ' ') {
                            cCol--;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;
                        // rook left movement

                        while (bRow < 7 && curBoard[++bRow][bCol].ID != ' ') {
                            cRow++;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;
                        // rook down movement

                        while (bCol > 0 && curBoard[--bRow][bCol].ID != ' ') {
                            cRow--;
                            cBoardCheck
                        }
                        cBoardCheck
                        // rook up movement
                        break;

                    case 'n':
                        cRow -= 2, cCol += 1;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow -= 2, cCol -= 1;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow += 2, cCol += 1;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow += 2, cCol -= 1;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow -= 1, cCol += 2;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow -= 1, cCol -= 2;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow += 1, cCol += 2;
                        cBoardCheck
                        cRow = i, cCol = j;

                        cRow += 1, cCol -= 2;
                        cBoardCheck
                        break;

                    case 'B':
                        while (bCol < 7 && bRow < 7 && curBoard[++bRow][++bCol].ID != ' ') {
                            cCol++, cRow++;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;

                        while (bRow > 0 && bCol > 0 && curBoard[--bRow][--bCol].ID != ' ') {
                            cCol--, cRow--;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;

                        while (bRow < 7 && bCol > 0 && curBoard[++bRow][--bCol].ID != ' ') {
                            cRow++, bCol--;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;

                        while (bRow > 0 && bCol < 7 && curBoard[--bRow][++bCol].ID != ' ') {
                            cRow--, cCol++;
                            cBoardCheck
                        }
                        cBoardCheck
                        break;

                    case 'Q':
                        while (bCol < 7 && curBoard[bRow][++bCol].ID != ' ') {
                            cCol++;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;
                        // rook right movement

                        while (bCol > 0 && curBoard[bRow][--bCol].ID != ' ') {
                            cCol--;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;
                        // rook left movement

                        while (bRow < 7 && curBoard[++bRow][bCol].ID != ' ') {
                            cRow++;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;
                        // rook down movement

                        while (bCol > 0 && curBoard[--bRow][bCol].ID != ' ') {
                            cRow--;
                            cBoardCheck
                        }
                        cBoardCheck
                        // rook up movement
                        bRow = i - 2, bCol = j - 2, cRow = i, cCol = j;

                        // horizontal/vertical movement

                        while (bCol < 7 && bRow < 7 && curBoard[++bRow][++bCol].ID != ' ') {
                            cCol++, cRow++;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;

                        while (bRow > 0 && bCol > 0 && curBoard[--bRow][--bCol].ID != ' ') {
                            cCol--, cRow--;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;

                        while (bRow < 7 && bCol > 0 && curBoard[++bRow][--bCol].ID != ' ') {
                            cRow++, bCol--;
                            cBoardCheck
                        }
                        cBoardCheck
                        bRow = i - 2; bCol = j - 2, cRow = i, cCol = j;;

                        while (bRow > 0 && bCol < 7 && curBoard[--bRow][++bCol].ID != ' ') {
                            cRow--, cCol++;
                            cBoardCheck
                        }
                        cBoardCheck
                        // diagonal movement
                        break;

                    default:
                        cRow--, cCol--;
                        cBoardCheck
                        cCol++;
                        cBoardCheck
                        cCol++;
                        cBoardCheck
                        cRow++;
                        cBoardCheck
                        cRow++;
                        cBoardCheck
                        cCol--;
                        cBoardCheck
                        cCol--;
                        cBoardCheck
                        cRow--;
                        cBoardCheck
                        break;
                }
            }
        }
    }
}

void boardUpdate(int curRow, int curCol, int targRow, int targCol) { // updates all 4 boards
    board[targRow][targCol] = board[curRow][curCol];
    board[curRow][curCol].ID = ' ';
    board[curRow][curCol].color = " ";
    board[curRow][curCol].canDoubleMove = false;

    simBoard[targRow][targCol] = board[targRow][targCol];
    simBoard[curRow][curCol] = board[curRow][curCol];
    // resets simulated board to actual board

    cBoardUpdate(cBoard, board);

    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            cSimBoard[i][j] = cBoard[i][j];
        }
    }
    // resets simulated control board to actual control board

    padBoard[curRow + 2][curCol + 2] = false;
    padBoard[targRow + 2][targCol + 2] = true;

}

void boardSetup() {
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

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            simBoard[i][j] = board[i][j];
        }
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            boardState.push_back(board[i][j].ID);
        }
    }
    moves2[boardState]++;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (i < 4 || i > 7 || j < 2 || j > 9) {
                padBoard[i][j] = true;
                cBoard[i][j] = BWCONT;
                cSimBoard[i][j] = BWCONT;
            }
            else {
                padBoard[i][j] = false;
                cBoard[i][j] = FREE;
                cSimBoard[i][j] = FREE;
            }
        }
    }
    cBoardUpdate(cBoard, board);
}

void showBoard() {
    for (int i = 0; i < 8; ++i) {
        std::cout << "---------------------------------\n";
        std::cout << "|";
        for (int j = 0; j < 8; ++j) {
            std::cout << " " << TXT << board[i][j].ID << " " << WC << "|";
        }
        std::cout << " " << abs(i - 8) << std::endl;
    }
    std::cout
    << "---------------------------------\n"
    << "  A   B   C   D   E   F   G   H\n" << std::endl;
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

bool checkDraw(const player& current) {
    if (moves2[boardState] > 2) {
        std::cout << "Draw by threefold repetition.\n";
        return true;
    }
    // if threefold repetition has occured

    if (white.strength == 0 && black.strength == 0) {
        std::cout << "Draw by insufficient material\n";
        return true;
    }
    // players only have king left

    if (white.pCount == 0 && (white.strength == 3 || white.strength == 3.5 || white.strength == 6) &&
        black.pCount == 0 && (black.strength == 3 || black.strength == 3.5 || black.strength == 6)) {
        std::cout << "Draw by insufficient material\n";
        return true;
    }
    // players only have king + knight / king + bishop + king + 2 knights

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].color == current.color && board[i][j].ID != ' ' && canMove(i, j, current)) {
                return false;
            }
        }
    }
    // if no pieces can move
    std::cout << "No pieces are able to move. The game ends in a draw.\n";
    return true;
}

bool inCheck(const player& current) {
    int king = cBoard[current.KY + 2][current.KX + 2];
    if (king == BWCONT || current.color == "White" ? king == BCONT : king == WCONT) {
        return true;
    }
    return false;
}

bool checkmate(const player& current) { // checked for at end of current players turn
    if (!inCheck(black) && !inCheck(white)) return false;
    // not checkmate if neither player is in check

    if (canMove(white.KY, white.KX, white) && canMove(black.KY, black.KX, black)) return false;
    // not checkmate if both kings are able to move

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j].isAttackingKing) {
                if (cBoard[i + 2][j + 2] == FREE || current.color == "White" ? cBoard[i + 2][j + 2] == WCONT : cBoard[i][j] == BCONT) return true;
            }
        }
    }
    return false;
}

bool simMove(const player& current, int curRow, int curCol, int targRow, int targCol) {
    simBoard[targRow][targCol] = simBoard[curRow][curCol];
    simBoard[curRow][curCol].ID = ' ';
    simBoard[curRow][curCol].color = " ";
    simBoard[curRow][curCol].canDoubleMove = false;
    cBoardUpdate(cSimBoard, simBoard);
    return true;
}

void move(player &current) {
    std::string inputPiece, inputMove;
    std::cout << current.color << "'s turn to move. ";
    reSelect:
    std::cout << "Input the piece you wish to move and it's location (PE1: pawn E1)\n";

    pieceSelect:
    std::cin >> inputPiece;

    int curRow = abs(inputPiece[2] - 56), curCol = inputPiece[1] - 65;

    if (!validPiece(inputPiece, current) || !canMove(curRow, curCol, current)) {
        std::cout << "Invalid input/piece is unable to move, please input the piece you wish to move again.\n";
        goto pieceSelect;
    }

    remove:
    std::cout << "Please specify where you would like to move your " << checkPiece(inputPiece[0]) << " (D4 to move to D4).\n";
    std::cout << "If you would like to select a different piece, type ""ESC""\n";

    pieceMove:
    std::cin >> inputMove;
    if (inputMove == "ESC") goto reSelect;
    if (!validMove(inputMove, curRow, curCol)) {
        std::cout << "Invalid input/piece can not move there, please input where you would like to move again.\n";
        goto pieceMove;
    }
    int targRow = abs(inputMove[1] - 56), targCol = inputMove[0] - 65;

    if (!simMove(current, curRow, curCol, targRow, targCol)) {
        std::cout << "Invalid move.\n";
        goto remove;
    }
    // if move would put player in check or player is already in check

    std::string move = inputPiece + inputMove;
    moves.push(move); // add current move to the stack

    if (board[curRow][curCol].ID == 'P') board[curRow][curCol].canDoubleMove = false; // if pawn was moved, disable

    if (board[curRow][curCol].ID == 'R' && curCol == 0) current.leftCastle = false;
    if (board[curRow][curCol].ID == 'R' && curCol == 7) current.rightCastle = false;

    if (board[curRow][curCol].ID == 'K') {
        current.KX = targCol;
        current.KY = targRow;
        current.leftCastle = false;
        current.rightCastle = false;
    } // if king was moved, update king coordinates and set canCastle to false

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

    boardUpdate(curRow, curCol, targRow, targCol);
    showBoard();
    // update state of board arrays

    int index = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            boardState[index] = board[i][j].ID;
            index++;
        }
    }
    moves2[boardState]++;
    // updates moves2 which keeps track of all positions that showed up on the board

    if (checkDraw(current.color == "White" ? black : white) || checkmate(current)) {
        gameOver = true;
        return;
    }
}

#endif //CHESS_CHESS_H
