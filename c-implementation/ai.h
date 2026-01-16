#ifndef AI_H
#define AI_H

#include "board.h"
#include <stdbool.h>

// Evaluation function pointer type
typedef double (*EvaluationFunc)(const Board *board);

// Default evaluation functions
double evaluate_standard(const Board *board);
double evaluate_ending(const Board *board);

// AI algorithms (decoupled from game logic)
double ai_minimax(Board *board, int depth, bool max_player, EvaluationFunc eval_func);
double ai_alpha_beta(Board *board, int depth, double alpha, double beta, bool max_player, bool forced_capture, EvaluationFunc eval_func);
Move ai_find_best_move(Board *board, int depth, bool forced_capture, EvaluationFunc eval_func);

// Helper for dynamic depth adjustment
int ai_determine_dynamic_depth(double time_previous_move, int depth, bool forced_capture, int num_moves);

#endif
