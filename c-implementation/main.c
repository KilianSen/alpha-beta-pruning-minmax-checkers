#include "board.h"
#include "ai.h"
#include "input.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

bool ending_conditions(Board *board, int figure_counter[2], bool forced_capture) {
    MoveList moves;
    board_generate_all_moves(board, forced_capture, &moves);
    
    int num_white, num_black;
    board_count_pieces(board, &num_white, &num_black);
    
    if (num_white == 0) {
        printf("Black won!\n");
        return true;
    }
    if (num_black == 0) {
        printf("White won!\n");
        return true;
    }
    
    // Check for draw (50 moves without capture)
    if (num_white + num_black == figure_counter[0]) {
        figure_counter[1]++;
        if (figure_counter[1] >= 50) {
            printf("Tie!\n");
            return true;
        }
    } else {
        figure_counter[0] = num_white + num_black;
        figure_counter[1] = 0;
    }
    
    if (moves.count == 0) {
        printf("There are no possible moves left! Game is finished!\n");
        return true;
    }
    
    return false;
}

int main(void) {
    // Initialize standard checkers board (1D array)
    char initial_board[BOARD_WIDTH * BOARD_HEIGHT] = {
        '.', 'c', '.', 'c', '.', 'c', '.', 'c',
        'c', '.', 'c', '.', 'c', '.', 'c', '.',
        '.', 'c', '.', 'c', '.', 'c', '.', 'c',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        'b', '.', 'b', '.', 'b', '.', 'b', '.',
        '.', 'b', '.', 'b', '.', 'b', '.', 'b',
        'b', '.', 'b', '.', 'b', '.', 'b', '.'
    };
    
    // Create board
    Board *board = board_create(BOARD_WIDTH, BOARD_HEIGHT);
    if (!board) {
        printf("Failed to create board!\n");
        return 1;
    }
    
    board_init(board, initial_board, true);
    
    // Game settings
    bool forced_capture = input_forced_moves();
    
    int depth = 6;
    double time_previous_move = 4.5;
    int without_capture[2] = {0, 0};
    
    printf("\n=== Checkers Game with Alpha-Beta Pruning ===\n");
    printf("White pieces: b (regular), B (king)\n");
    printf("Black pieces: c (regular), C (king)\n");
    printf("White moves first (you play as white)\n\n");
    
    // Main game loop
    while (1) {
        // Check ending conditions
        if (ending_conditions(board, without_capture, forced_capture)) {
            break;
        }
        
        printf("\nCurrent depth: %d\n", depth);
        
        // Find available capturing pieces
        MoveList available_pieces;
        board_find_capturing_moves(board, &available_pieces);
        
        // Display board
        if (forced_capture && available_pieces.count > 0) {
            print_board(board, &available_pieces, NULL);
        } else {
            print_board(board, NULL, NULL);
        }
        
        // Player's turn (white)
        printf("--- Your turn (White) ---\n");
        
        Coordinate piece;
        if (forced_capture && available_pieces.count > 0) {
            if (!input_choose_piece(board, &available_pieces, &piece)) {
                printf("Goodbye!\n");
                break;
            }
        } else {
            if (!input_choose_piece(board, NULL, &piece)) {
                printf("Goodbye!\n");
                break;
            }
        }
        
        // Get valid moves for chosen piece
        MoveList valid_moves;
        board_find_valid_moves_for_piece(board, piece, forced_capture, &valid_moves);
        
        // Create move list for display
        MoveList selected_list = {0};
        selected_list.moves[0].from = piece;
        selected_list.moves[0].to = piece;
        selected_list.count = 1;
        
        print_board(board, &selected_list, &valid_moves);
        
        // Choose destination
        Coordinate new_position;
        if (!input_choose_field(&valid_moves, &new_position)) {
            printf("Move cancelled! Goodbye!\n");
            break;
        }
        
        // Save previous board state
        Board *previous_board = board_create(BOARD_WIDTH, BOARD_HEIGHT);
        board_copy(previous_board, board);
        
        // Apply move
        Move player_move = {piece, new_position, false};
        // Check if it's a capture
        if (abs(piece.row - new_position.row) == 2) {
            player_move.is_capture = true;
        }
        board_apply_move(board, &player_move);
        
        // Show the move
        MoveList differences;
        board_find_differences(board, previous_board, &differences);
        print_board(board, &differences, NULL);
        printf("User played the move displayed on the table above.\n\n");
        
        board_free(previous_board);
        
        // Check ending conditions again
        if (ending_conditions(board, without_capture, forced_capture)) {
            break;
        }
        
        // Computer's turn (black)
        printf("--- Computer's turn (Black) ---\n");
        printf("THINKING...\n");
        
        MoveList computer_moves;
        board_generate_all_moves(board, forced_capture, &computer_moves);
        int num_moves = computer_moves.count;
        
        depth = ai_determine_dynamic_depth(time_previous_move, depth, forced_capture, num_moves);
        
        previous_board = board_create(BOARD_WIDTH, BOARD_HEIGHT);
        board_copy(previous_board, board);
        
        clock_t start = clock();
        
        int num_white, num_black;
        board_count_pieces(board, &num_white, &num_black);
        
        Move best_move;
        if (num_white + num_black > 6) {
            // Use standard evaluation
            best_move = ai_find_best_move(board, depth, forced_capture, evaluate_standard);
        } else {
            // Use ending evaluation with deeper search
            best_move = ai_find_best_move(board, 20, forced_capture, evaluate_ending);
        }
        
        board_apply_move(board, &best_move);
        
        clock_t end = clock();
        time_previous_move = (double)(end - start) / CLOCKS_PER_SEC;
        
        printf("Time taken: %.2f seconds\n", time_previous_move);
        
        board_find_differences(board, previous_board, &differences);
        print_board(board, &differences, NULL);
        printf("Computer played a move displayed on the table above.\n\n");
        
        board_free(previous_board);
    }
    
    board_free(board);
    
    printf("\n=== Game Over ===\n");
    return 0;
}
