#ifndef INPUT_H
#define INPUT_H

#include "board.h"
#include <stdbool.h>

bool input_choose_piece(const Board *board, const MoveList *available_pieces, Coordinate *chosen);
bool input_choose_field(const MoveList *valid_moves, Coordinate *chosen);
bool input_forced_moves(void);

#endif
