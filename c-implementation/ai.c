#include "ai.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Standard evaluation function
double evaluate_standard(const Board *board) {
    int white_value = 0;
    int black_value = 0;
    int num_white = 0;
    int num_black = 0;
    
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            char piece = board_get(board, i, j);
            
            if (piece == 'b') {
                num_white++;
                // Center control bonus
                if (2 < i && i < 5 && 1 < j && j < 6) {
                    white_value += 50;
                } else if (i < 4) {
                    white_value += 45;
                } else {
                    white_value += 40;
                }
            } else if (piece == 'B') {
                num_white++;
                white_value += 60;
            } else if (piece == 'c') {
                num_black++;
                // Center control bonus
                if (2 < i && i < 5 && 1 < j && j < 6) {
                    black_value += 50;
                } else if (i > 3) {
                    black_value += 45;
                } else {
                    black_value += 40;
                }
            } else if (piece == 'C') {
                num_black++;
                black_value += 60;
            }
        }
    }
    
    double evaluation = black_value - white_value;
    
    if (num_white == 0) {
        evaluation = INFINITY;
    }
    if (num_black == 0) {
        evaluation = -INFINITY;
    }
    
    return evaluation;
}

// Ending game evaluation function (simpler, piece count focused)
double evaluate_ending(const Board *board) {
    int white_value = 0;
    int black_value = 0;
    
    for (int i = 0; i < board->height; i++) {
        for (int j = 0; j < board->width; j++) {
            char piece = board_get(board, i, j);
            
            if (piece == 'b') {
                white_value += 2;
            } else if (piece == 'B') {
                white_value += 3;
            } else if (piece == 'c') {
                black_value += 2;
            } else if (piece == 'C') {
                black_value += 3;
            }
        }
    }
    
    return black_value - white_value;
}

// Simple minimax algorithm
double ai_minimax(Board *board, int depth, bool max_player, EvaluationFunc eval_func) {
    if (depth == 0 || board_is_game_over(board)) {
        return eval_func(board);
    }
    
    MoveList moves;
    board_generate_all_moves(board, false, &moves);
    
    if (max_player) {
        double max_eval = -INFINITY;
        for (int i = 0; i < moves.count; i++) {
            Board *child = board_create(board->width, board->height);
            board_copy(child, board);
            board_apply_move(child, &moves.moves[i]);
            
            double eval = ai_minimax(child, depth - 1, false, eval_func);
            if (eval > max_eval) {
                max_eval = eval;
            }
            
            board_free(child);
        }
        return max_eval;
    } else {
        double min_eval = INFINITY;
        for (int i = 0; i < moves.count; i++) {
            Board *child = board_create(board->width, board->height);
            board_copy(child, board);
            board_apply_move(child, &moves.moves[i]);
            
            double eval = ai_minimax(child, depth - 1, true, eval_func);
            if (eval < min_eval) {
                min_eval = eval;
            }
            
            board_free(child);
        }
        return min_eval;
    }
}

// Alpha-beta pruning algorithm (decoupled from game logic)
double ai_alpha_beta(Board *board, int depth, double alpha, double beta, bool max_player, bool forced_capture, EvaluationFunc eval_func) {
    if (depth == 0 || board_is_game_over(board)) {
        return eval_func(board);
    }
    
    MoveList moves;
    board_generate_all_moves(board, forced_capture, &moves);
    
    if (max_player) {
        double max_eval = -INFINITY;
        for (int i = 0; i < moves.count; i++) {
            Board *child = board_create(board->width, board->height);
            board_copy(child, board);
            board_apply_move(child, &moves.moves[i]);
            
            double eval = ai_alpha_beta(child, depth - 1, alpha, beta, false, forced_capture, eval_func);
            
            board_free(child);
            
            if (eval > max_eval) {
                max_eval = eval;
            }
            if (eval > alpha) {
                alpha = eval;
            }
            if (beta <= alpha) {
                break;  // Beta cutoff
            }
        }
        return max_eval;
    } else {
        double min_eval = INFINITY;
        for (int i = 0; i < moves.count; i++) {
            Board *child = board_create(board->width, board->height);
            board_copy(child, board);
            board_apply_move(child, &moves.moves[i]);
            
            double eval = ai_alpha_beta(child, depth - 1, alpha, beta, true, forced_capture, eval_func);
            
            board_free(child);
            
            if (eval < min_eval) {
                min_eval = eval;
            }
            if (eval < beta) {
                beta = eval;
            }
            if (beta <= alpha) {
                break;  // Alpha cutoff
            }
        }
        return min_eval;
    }
}

// Find the best move for the current player
Move ai_find_best_move(Board *board, int depth, bool forced_capture, EvaluationFunc eval_func) {
    MoveList moves;
    board_generate_all_moves(board, forced_capture, &moves);
    
    Move best_move = moves.moves[0];
    double best_eval = -INFINITY;
    
    for (int i = 0; i < moves.count; i++) {
        Board *child = board_create(board->width, board->height);
        board_copy(child, board);
        board_apply_move(child, &moves.moves[i]);
        
        // Assuming we're maximizing (computer is black)
        double eval = ai_alpha_beta(child, depth - 1, -INFINITY, INFINITY, false, forced_capture, eval_func);
        
        board_free(child);
        
        if (eval > best_eval) {
            best_eval = eval;
            best_move = moves.moves[i];
        }
    }
    
    return best_move;
}

// Dynamic depth adjustment based on game state
int ai_determine_dynamic_depth(double time_previous_move, int depth, bool forced_capture, int num_moves) {
    if (forced_capture) {
        if (time_previous_move < 0.5 && num_moves <= 6) {
            return depth + 1;
        }
        if (depth > 6 && (time_previous_move > 4 || num_moves > 6)) {
            return depth - 1;
        }
        return depth;
    } else {
        if (time_previous_move < 0.5) {
            return depth + 1;
        }
        if (time_previous_move > 4.5) {
            return depth - 1;
        }
        return depth;
    }
}
