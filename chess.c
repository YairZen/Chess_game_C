#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8

typedef enum { false, true } bool;
typedef enum { EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING } PieceType;
typedef enum { NONE, WHITE, BLACK } Color;

typedef struct {
    PieceType type;
    Color color;
} Piece;

typedef struct {
    Piece board[BOARD_SIZE][BOARD_SIZE];
    Color turn;
} GameState;

void initializeBoard(GameState* game);
void printBoard(GameState* game);
bool movePiece(GameState* game, int srcRow, int srcCol, int destRow, int destCol);
bool isValidMove(GameState* game, int srcRow, int srcCol, int destRow, int destCol);
void switchTurn(GameState* game);

int main() {
    GameState game;
    initializeBoard(&game);

    while (true) {
        printBoard(&game);
        int srcRow, srcCol, destRow, destCol;
        printf("Enter move (srcRow srcCol destRow destCol): ");
        scanf("%d %d %d %d", &srcRow, &srcCol, &destRow, &destCol);
        
        if (movePiece(&game, srcRow, srcCol, destRow, destCol)) {
            switchTurn(&game);
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    return 0;
}

void initializeBoard(GameState* game) {
    memset(game->board, 0, sizeof(game->board));
    
    // Initialize pawns
    for (int i = 0; i < BOARD_SIZE; i++) {
        game->board[1][i].type = PAWN;
        game->board[1][i].color = BLACK;
        game->board[6][i].type = PAWN;
        game->board[6][i].color = WHITE;
    }

    // Initialize other pieces
    PieceType majorPieces[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
    for (int i = 0; i < BOARD_SIZE; i++) {
        game->board[0][i].type = majorPieces[i];
        game->board[0][i].color = BLACK;
        game->board[7][i].type = majorPieces[i];
        game->board[7][i].color = WHITE;
    }

    game->turn = WHITE;
}

void printBoard(GameState* game) {
    printf("  a b c d e f g h\n");
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("%d ", 8 - row);
        for (int col = 0; col < BOARD_SIZE; col++) {
            Piece piece = game->board[row][col];
            char symbol = '.';
            if (piece.type != EMPTY) {
                symbol = (piece.color == WHITE) ? 'W' : 'B';
                switch (piece.type) {
                    case PAWN:   symbol += 'P' - 'A'; break;
                    case ROOK:   symbol += 'R' - 'A'; break;
                    case KNIGHT: symbol += 'N' - 'A'; break;
                    case BISHOP: symbol += 'B' - 'A'; break;
                    case QUEEN:  symbol += 'Q' - 'A'; break;
                    case KING:   symbol += 'K' - 'A'; break;
                    default: break;
                }
            }
            printf("%c ", symbol);
        }
        printf("%d\n", 8 - row);
    }
    printf("  a b c d e f g h\n");
    printf("Turn: %s\n", (game->turn == WHITE) ? "White" : "Black");
}

bool movePiece(GameState* game, int srcRow, int srcCol, int destRow, int destCol) {
    if (!isValidMove(game, srcRow, srcCol, destRow, destCol)) {
        return false;
    }

    game->board[destRow][destCol] = game->board[srcRow][srcCol];
    game->board[srcRow][srcCol].type = EMPTY;
    game->board[srcRow][srcCol].color = NONE;

    return true;
}

bool isValidMove(GameState* game, int srcRow, int srcCol, int destRow, int destCol) {
    // Basic out-of-bounds and empty source check
    if (srcRow < 0 || srcRow >= BOARD_SIZE || srcCol < 0 || srcCol >= BOARD_SIZE ||
        destRow < 0 || destRow >= BOARD_SIZE || destCol < 0 || destCol >= BOARD_SIZE) {
        return false;
    }

    Piece srcPiece = game->board[srcRow][srcCol];
    Piece destPiece = game->board[destRow][destCol];

    if (srcPiece.type == EMPTY || srcPiece.color != game->turn) {
        return false;
    }

    if (destPiece.type != EMPTY && destPiece.color == game->turn) {
        return false;
    }

    // Simplified movement rules for demonstration purposes
    int rowDiff = abs(destRow - srcRow);
    int colDiff = abs(destCol - srcCol);

    switch (srcPiece.type) {
        case PAWN:
            if (srcPiece.color == WHITE) {
                if (srcRow == 6 && destRow == 4 && colDiff == 0 && destPiece.type == EMPTY) {
                    return true;
                }
                if (destRow == srcRow - 1 && colDiff == 0 && destPiece.type == EMPTY) {
                    return true;
                }
                if (destRow == srcRow - 1 && colDiff == 1 && destPiece.type != EMPTY) {
                    return true;
                }
            } else {
                if (srcRow == 1 && destRow == 3 && colDiff == 0 && destPiece.type == EMPTY) {
                    return true;
                }
                if (destRow == srcRow + 1 && colDiff == 0 && destPiece.type == EMPTY) {
                    return true;
                }
                if (destRow == srcRow + 1 && colDiff == 1 && destPiece.type != EMPTY) {
                    return true;
                }
            }
            break;
        case ROOK:
            if (rowDiff == 0 || colDiff == 0) {
                return true;
            }
            break;
        case KNIGHT:
            if (rowDiff * colDiff == 2) {
                return true;
            }
            break;
        case BISHOP:
            if (rowDiff == colDiff) {
                return true;
            }
            break;
        case QUEEN:
            if (rowDiff == colDiff || rowDiff == 0 || colDiff == 0) {
                return true;
            }
            break;
        case KING:
            if (rowDiff <= 1 && colDiff <= 1) {
                return true;
            }
            break;
        default:
            return false;
    }

    return false;
}

void switchTurn(GameState* game) {
    game->turn = (game->turn == WHITE) ? BLACK : WHITE;
}
