# C Implementation of Alpha-Beta Pruning Minmax Checkers

This is a C implementation of the checkers game with AI using the minimax algorithm with alpha-beta pruning optimization.

## Architecture

The implementation follows a **modular, decoupled architecture**:

### Board Representation
- Uses a **1D array** with configurable width and height (not hardcoded 2D array)
- Board state is separated from AI logic
- Helper functions provide convenient access: `board_get(board, row, col)` and `board_set(board, row, col, value)`

### Modules

1. **board.c/h** - Game Logic (Decoupled from AI)
   - Board state management with 1D array
   - Move generation and validation
   - Piece counting and game state evaluation
   - No AI algorithms - pure game logic

2. **ai.c/h** - AI Algorithms (Decoupled from Game)
   - Minimax algorithm implementation
   - Alpha-beta pruning optimization
   - Evaluation functions (standard and endgame)
   - Best move selection
   - Takes evaluation functions as parameters for flexibility

3. **input.c/h** - User Input
   - Piece selection
   - Move selection
   - Game configuration

4. **output.c/h** - Display
   - Board visualization with ANSI colors
   - Converts 1D array to 2D display

5. **main.c** - Game Loop
   - Orchestrates game flow
   - Manages turns between player and AI
   - Handles game ending conditions

## Key Features

### 1. 1D Array Board Representation
```c
Board *board = board_create(width, height);
char value = board_get(board, row, col);
board_set(board, row, col, 'b');
```

### 2. Decoupled AI
The AI algorithms are completely independent of the board implementation:
```c
// AI takes evaluation function as parameter
Move best_move = ai_find_best_move(board, depth, forced_capture, evaluate_standard);

// Can easily switch evaluation strategies
Move endgame_move = ai_find_best_move(board, depth, forced_capture, evaluate_ending);
```

### 3. Minimax with Alpha-Beta Pruning
- Standard minimax algorithm
- Alpha-beta pruning for optimization
- Dynamic depth adjustment based on game state
- Separate evaluation functions for mid-game and endgame

## Building

```bash
cd c-implementation
make
```

## Running

```bash
./checkers
```

Or:

```bash
make run
```

## Cleaning

```bash
make clean
```

## Game Rules

- **White pieces** (b/B): Player controlled, moves up the board
- **Black pieces** (c/C): AI controlled, moves down the board
- **Regular pieces** (b/c): Can only move forward
- **Kings** (B/C): Can move in all diagonal directions
- **Forced captures**: Optional rule that requires capturing when possible

## Code Structure

```
c-implementation/
├── board.h         - Board API with 1D array
├── board.c         - Board implementation (game logic)
├── ai.h            - AI API (decoupled)
├── ai.c            - AI algorithms (minimax, alpha-beta)
├── input.h         - Input handling API
├── input.c         - User input implementation
├── output.h        - Display API
├── output.c        - Board visualization
├── main.c          - Main game loop
├── Makefile        - Build configuration
└── README.md       - This file
```

## Design Principles

1. **Separation of Concerns**: Game logic (board) is separated from AI logic
2. **Flexibility**: 1D array with width/height allows for different board sizes
3. **Modularity**: Each module has a clear, single responsibility
4. **Extensibility**: Easy to add new evaluation functions or AI strategies
5. **Standard C99**: Uses standard C with minimal dependencies

## Advantages over Python Version

1. **Performance**: C is significantly faster than Python
2. **Memory Efficiency**: Direct control over memory allocation
3. **Portability**: Compiles on any platform with a C compiler
4. **Decoupled Design**: AI can be used with different game implementations
5. **Flexible Board**: 1D array representation is more memory efficient and allows variable board sizes
