#include "output.h"
#include <stdio.h>
#include <stdbool.h>

// ANSI color codes
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_CYAN "\033[36m"
#define COLOR_BLACK "\033[30;1m"
#define COLOR_WHITE "\033[37;1m"
#define BG_BLUE "\033[44;1m"
#define BG_WHITE "\033[47;1m"
#define BG_PINK "\033[45m"
#define COLOR_END "\033[0m"

static bool is_in_list(const MoveList *list, int row, int col) {
    if (!list) return false;
    
    for (int i = 0; i < list->count; i++) {
        if ((list->moves[i].from.row == row && list->moves[i].from.col == col) ||
            (list->moves[i].to.row == row && list->moves[i].to.col == col)) {
            return true;
        }
    }
    return false;
}

void print_board(const Board *board, const MoveList *selected, const MoveList *valid_moves) {
    // Print column headers
    printf("   ");
    for (int j = 0; j < board->width; j++) {
        printf("  %d  ", j);
    }
    printf("\n");
    
    // Print top border
    printf("  ");
    for (int j = 0; j < board->width; j++) {
        printf("_____");
    }
    printf("\n");
    
    // Print each row
    for (int i = 0; i < board->height; i++) {
        printf("%d |", i);
        
        for (int j = 0; j < board->width; j++) {
            char piece = board_get(board, i, j);
            bool is_selected = is_in_list(selected, i, j);
            bool is_valid_move = is_in_list(valid_moves, i, j);
            
            if (piece == 'b' || piece == 'B') {
                if (is_selected) {
                    printf(BG_BLUE COLOR_BLACK " %c " COLOR_END, piece);
                } else {
                    printf(COLOR_GREEN " %c " COLOR_END, piece);
                }
            } else if (piece == 'c' || piece == 'C') {
                if (is_selected) {
                    printf(BG_BLUE COLOR_BLACK " %c " COLOR_END, piece);
                } else {
                    printf(COLOR_RED " %c " COLOR_END, piece);
                }
            } else if (is_valid_move) {
                printf(BG_WHITE COLOR_BLACK "%d %d" COLOR_END, i, j);
            } else {
                if (is_selected) {
                    printf(BG_PINK COLOR_BLACK " %c " COLOR_END, piece);
                } else {
                    printf(" %c ", piece);
                }
            }
            printf("  ");
        }
        
        printf("| %d\n", i);
    }
    
    // Print bottom border
    printf("  ");
    for (int j = 0; j < board->width; j++) {
        printf("-----");
    }
    printf("\n");
    
    // Print column headers again
    printf("   ");
    for (int j = 0; j < board->width; j++) {
        printf("  %d  ", j);
    }
    printf("\n\n");
}
