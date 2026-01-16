#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool input_choose_piece(const Board *board, const MoveList *available_pieces, Coordinate *chosen) {
    char input[100];
    
    while (1) {
        if (available_pieces && available_pieces->count > 0) {
            printf("Forced captures are enabled! You can only choose the highlighted figures.\n");
        }
        printf("Enter the figure coordinates (row+column without space ex. 70 for row 7, col 0) <x to exit>: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            return false;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Check for exit
        if (strlen(input) > 0 && tolower(input[0]) == 'x') {
            return false;
        }
        
        // Parse coordinate
        if (strlen(input) >= 2 && isdigit(input[0]) && isdigit(input[1])) {
            int coord = atoi(input);
            chosen->row = coord / 10;
            chosen->col = coord % 10;
            
            // Validate bounds
            if (chosen->row < 0 || chosen->row >= board->height || 
                chosen->col < 0 || chosen->col >= board->width) {
                printf("Coordinate out of bounds! Try again.\n");
                continue;
            }
            
            char piece = board_get(board, chosen->row, chosen->col);
            
            // Check if piece exists and belongs to current player
            bool is_white = (piece == 'b' || piece == 'B');
            bool is_black = (piece == 'c' || piece == 'C');
            
            if (board->white_to_move && !is_white) {
                printf("Selection is not valid! Choose a white piece (b or B).\n");
                continue;
            }
            
            if (!board->white_to_move && !is_black) {
                printf("Selection is not valid! Choose a black piece (c or C).\n");
                continue;
            }
            
            // If forced capture, check if piece is in available list
            if (available_pieces && available_pieces->count > 0) {
                bool found = false;
                for (int i = 0; i < available_pieces->count; i++) {
                    if (available_pieces->moves[i].from.row == chosen->row && 
                        available_pieces->moves[i].from.col == chosen->col) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    printf("This piece cannot capture! Choose a highlighted piece.\n");
                    continue;
                }
            }
            
            // Check if piece has valid moves
            MoveList moves;
            board_find_valid_moves_for_piece(board, *chosen, false, &moves);
            if (moves.count == 0) {
                printf("Chosen figure has no available moves!\n");
                continue;
            }
            
            return true;
        }
        
        printf("Invalid coordinate! Try again.\n");
    }
}

bool input_choose_field(const MoveList *valid_moves, Coordinate *chosen) {
    char input[100];
    
    while (1) {
        printf("Enter the field coordinates (row+column without space ex. 70 for row 7, col 0) <x to exit>: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            return false;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Check for exit
        if (strlen(input) > 0 && tolower(input[0]) == 'x') {
            return false;
        }
        
        // Parse coordinate
        if (strlen(input) >= 2 && isdigit(input[0]) && isdigit(input[1])) {
            int coord = atoi(input);
            chosen->row = coord / 10;
            chosen->col = coord % 10;
            
            // Check if coordinate is in valid moves
            bool found = false;
            for (int i = 0; i < valid_moves->count; i++) {
                if (valid_moves->moves[i].to.row == chosen->row && 
                    valid_moves->moves[i].to.col == chosen->col) {
                    found = true;
                    break;
                }
            }
            
            if (found) {
                return true;
            }
            
            printf("Selection is not valid! Try again.\n");
            continue;
        }
        
        printf("Invalid coordinate! Try again.\n");
    }
}

bool input_forced_moves(void) {
    char input[100];
    
    while (1) {
        printf("Do you want to enable forced captures? <yes|no>: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            return false;
        }
        
        // Remove newline and convert to lowercase
        input[strcspn(input, "\n")] = 0;
        for (int i = 0; input[i]; i++) {
            input[i] = tolower(input[i]);
        }
        
        if (strcmp(input, "yes") == 0) {
            return true;
        }
        if (strcmp(input, "no") == 0) {
            return false;
        }
        
        printf("Invalid choice! Try again.\n");
    }
}
