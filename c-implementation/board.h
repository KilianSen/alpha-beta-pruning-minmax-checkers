#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define MAX_MOVES 100

typedef struct Board {
    char *cells;          // 1D array of cells
    int width;
    int height;
    bool white_to_move;
    bool game_end;
} Board;

typedef struct Coordinate {
    int row;
    int col;
} Coordinate;

typedef struct Move {
    Coordinate from;
    Coordinate to;
    bool is_capture;
} Move;

typedef struct MoveList {
    Move moves[MAX_MOVES];
    int count;
} MoveList;

// Board initialization and cleanup
Board* board_create(int width, int height);
void board_init(Board *board, const char *cells, bool white_to_move);
void board_free(Board *board);
void board_copy(Board *dest, const Board *src);

// Board access helpers (1D array indexing)
static inline int board_index(const Board *board, int row, int col) {
    return row * board->width + col;
}

static inline char board_get(const Board *board, int row, int col) {
    return board->cells[board_index(board, row, col)];
}

static inline void board_set(Board *board, int row, int col, char value) {
    board->cells[board_index(board, row, col)] = value;
}

// Game logic functions
void board_count_pieces(const Board *board, int *num_white, int *num_black);
void board_find_valid_moves_for_piece(const Board *board, Coordinate coord, bool forced, MoveList *moves);
void board_find_capturing_moves(const Board *board, MoveList *pieces);
void board_generate_all_moves(const Board *board, bool forced_capture, MoveList *moves);
bool board_apply_move(Board *board, const Move *move);
bool board_is_game_over(const Board *board);
void board_find_differences(const Board *current, const Board *previous, MoveList *differences);

#endif
