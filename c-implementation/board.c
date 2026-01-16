#include "board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Board* board_create(int width, int height) {
    Board *board = (Board*)malloc(sizeof(Board));
    if (!board) return NULL;
    
    board->cells = (char*)malloc(width * height * sizeof(char));
    if (!board->cells) {
        free(board);
        return NULL;
    }
    
    board->width = width;
    board->height = height;
    board->white_to_move = true;
    board->game_end = false;
    
    return board;
}

void board_init(Board *board, const char *cells, bool white_to_move) {
    int size = board->width * board->height;
    memcpy(board->cells, cells, size);
    board->white_to_move = white_to_move;
    board->game_end = false;
}

void board_free(Board *board) {
    if (board) {
        free(board->cells);
        free(board);
    }
}

void board_copy(Board *dest, const Board *src) {
    int size = src->width * src->height;
    memcpy(dest->cells, src->cells, size);
    dest->width = src->width;
    dest->height = src->height;
    dest->white_to_move = src->white_to_move;
    dest->game_end = src->game_end;
}

void board_count_pieces(const Board *board, int *num_white, int *num_black) {
    *num_white = 0;
    *num_black = 0;
    
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            char piece = board_get(board, i, j);
            if (piece == 'b' || piece == 'B') {
                (*num_white)++;
            }
            if (piece == 'c' || piece == 'C') {
                (*num_black)++;
            }
        }
    }
}

void board_find_valid_moves_for_piece(const Board *board, Coordinate coord, bool forced, MoveList *moves) {
    moves->count = 0;
    MoveList captures = {0};
    MoveList regular = {0};
    
    char figure = board_get(board, coord.row, coord.col);
    if (figure == '.') return;
    
    // Check if piece belongs to current player
    bool is_white = (figure == 'b' || figure == 'B');
    if (is_white != board->white_to_move) return;
    
    // Kings and black pieces (b/B) can move down (increasing row)
    if (figure != 'c') {
        if (coord.row < board->height - 1) {
            // Down-left
            if (coord.col - 1 >= 0) {
                char target = board_get(board, coord.row + 1, coord.col - 1);
                if (target == '.') {
                    Move move = {{coord.row, coord.col}, {coord.row + 1, coord.col - 1}, false};
                    regular.moves[regular.count++] = move;
                } else if (coord.row + 2 < board->height && coord.col - 2 >= 0) {
                    char jump_target = board_get(board, coord.row + 2, coord.col - 2);
                    if (jump_target == '.') {
                        bool target_is_white = (target == 'b' || target == 'B');
                        if (is_white != target_is_white && target != '.') {
                            Move move = {{coord.row, coord.col}, {coord.row + 2, coord.col - 2}, true};
                            captures.moves[captures.count++] = move;
                        }
                    }
                }
            }
            // Down-right
            if (coord.col + 1 < board->width) {
                char target = board_get(board, coord.row + 1, coord.col + 1);
                if (target == '.') {
                    Move move = {{coord.row, coord.col}, {coord.row + 1, coord.col + 1}, false};
                    regular.moves[regular.count++] = move;
                } else if (coord.row + 2 < board->height && coord.col + 2 < board->width) {
                    char jump_target = board_get(board, coord.row + 2, coord.col + 2);
                    if (jump_target == '.') {
                        bool target_is_white = (target == 'b' || target == 'B');
                        if (is_white != target_is_white && target != '.') {
                            Move move = {{coord.row, coord.col}, {coord.row + 2, coord.col + 2}, true};
                            captures.moves[captures.count++] = move;
                        }
                    }
                }
            }
        }
    }
    
    // Kings and black pieces (c/C) can move up (decreasing row)
    if (figure != 'b') {
        if (coord.row > 0) {
            // Up-left
            if (coord.col - 1 >= 0) {
                char target = board_get(board, coord.row - 1, coord.col - 1);
                if (target == '.') {
                    Move move = {{coord.row, coord.col}, {coord.row - 1, coord.col - 1}, false};
                    regular.moves[regular.count++] = move;
                } else if (coord.row - 2 >= 0 && coord.col - 2 >= 0) {
                    char jump_target = board_get(board, coord.row - 2, coord.col - 2);
                    if (jump_target == '.') {
                        bool target_is_white = (target == 'b' || target == 'B');
                        if (is_white != target_is_white && target != '.') {
                            Move move = {{coord.row, coord.col}, {coord.row - 2, coord.col - 2}, true};
                            captures.moves[captures.count++] = move;
                        }
                    }
                }
            }
            // Up-right
            if (coord.col + 1 < board->width) {
                char target = board_get(board, coord.row - 1, coord.col + 1);
                if (target == '.') {
                    Move move = {{coord.row, coord.col}, {coord.row - 1, coord.col + 1}, false};
                    regular.moves[regular.count++] = move;
                } else if (coord.row - 2 >= 0 && coord.col + 2 < board->width) {
                    char jump_target = board_get(board, coord.row - 2, coord.col + 2);
                    if (jump_target == '.') {
                        bool target_is_white = (target == 'b' || target == 'B');
                        if (is_white != target_is_white && target != '.') {
                            Move move = {{coord.row, coord.col}, {coord.row - 2, coord.col + 2}, true};
                            captures.moves[captures.count++] = move;
                        }
                    }
                }
            }
        }
    }
    
    // If forced and captures exist, only return captures
    if (forced && captures.count > 0) {
        memcpy(moves, &captures, sizeof(MoveList));
    } else {
        // Return captures first, then regular moves
        for (int i = 0; i < captures.count; i++) {
            moves->moves[moves->count++] = captures.moves[i];
        }
        for (int i = 0; i < regular.count; i++) {
            moves->moves[moves->count++] = regular.moves[i];
        }
    }
}

void board_find_capturing_moves(const Board *board, MoveList *pieces) {
    pieces->count = 0;
    
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            char piece = board_get(board, i, j);
            
            if (board->white_to_move && (piece == 'b' || piece == 'B')) {
                Coordinate coord = {i, j};
                MoveList move_list;
                board_find_valid_moves_for_piece(board, coord, false, &move_list);
                
                for (int k = 0; k < move_list.count; k++) {
                    if (move_list.moves[k].is_capture) {
                        pieces->moves[pieces->count].from = coord;
                        pieces->moves[pieces->count].to = coord;
                        pieces->count++;
                        break;
                    }
                }
            }
            
            if (!board->white_to_move && (piece == 'c' || piece == 'C')) {
                Coordinate coord = {i, j};
                MoveList move_list;
                board_find_valid_moves_for_piece(board, coord, false, &move_list);
                
                for (int k = 0; k < move_list.count; k++) {
                    if (move_list.moves[k].is_capture) {
                        pieces->moves[pieces->count].from = coord;
                        pieces->moves[pieces->count].to = coord;
                        pieces->count++;
                        break;
                    }
                }
            }
        }
    }
}

void board_generate_all_moves(const Board *board, bool forced_capture, MoveList *moves) {
    moves->count = 0;
    MoveList captures = {0};
    MoveList all = {0};
    
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            char piece = board_get(board, i, j);
            bool is_white = (piece == 'b' || piece == 'B');
            
            if ((board->white_to_move && is_white) || (!board->white_to_move && !is_white && piece != '.')) {
                Coordinate coord = {i, j};
                MoveList piece_moves;
                board_find_valid_moves_for_piece(board, coord, false, &piece_moves);
                
                for (int k = 0; k < piece_moves.count; k++) {
                    if (piece_moves.moves[k].is_capture) {
                        captures.moves[captures.count++] = piece_moves.moves[k];
                    } else {
                        all.moves[all.count++] = piece_moves.moves[k];
                    }
                }
            }
        }
    }
    
    // If forced capture and captures exist, only return captures
    if (forced_capture && captures.count > 0) {
        memcpy(moves, &captures, sizeof(MoveList));
    } else {
        // Return all moves (captures + regular)
        for (int i = 0; i < captures.count; i++) {
            moves->moves[moves->count++] = captures.moves[i];
        }
        for (int i = 0; i < all.count; i++) {
            moves->moves[moves->count++] = all.moves[i];
        }
    }
}

bool board_apply_move(Board *board, const Move *move) {
    char piece = board_get(board, move->from.row, move->from.col);
    if (piece == '.') return false;
    
    // Handle capture
    if (move->is_capture) {
        int mid_row = (move->from.row + move->to.row) / 2;
        int mid_col = (move->from.col + move->to.col) / 2;
        board_set(board, mid_row, mid_col, '.');
    }
    
    // Handle promotion
    if ((piece == 'b' || piece == 'B') && move->to.row == 0) {
        piece = 'B';
    }
    if ((piece == 'c' || piece == 'C') && move->to.row == board->height - 1) {
        piece = 'C';
    }
    
    // Move piece
    board_set(board, move->to.row, move->to.col, piece);
    board_set(board, move->from.row, move->from.col, '.');
    
    // Switch player
    board->white_to_move = !board->white_to_move;
    
    return true;
}

bool board_is_game_over(const Board *board) {
    if (board->game_end) return true;
    
    int num_white, num_black;
    board_count_pieces(board, &num_white, &num_black);
    
    if (num_white == 0 || num_black == 0) {
        return true;
    }
    
    // Check if current player has any moves
    MoveList moves;
    board_generate_all_moves(board, false, &moves);
    
    return moves.count == 0;
}

void board_find_differences(const Board *current, const Board *previous, MoveList *differences) {
    differences->count = 0;
    
    for (int i = 0; i < current->height; i++) {
        for (int j = 0; j < current->width; j++) {
            if (board_get(current, i, j) != board_get(previous, i, j)) {
                differences->moves[differences->count].from.row = i;
                differences->moves[differences->count].from.col = j;
                differences->moves[differences->count].to.row = i;
                differences->moves[differences->count].to.col = j;
                differences->count++;
            }
        }
    }
}
