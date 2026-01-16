# alpha-beta-pruning-minmax-checkers
Minmax algorithm with Alpha-Beta (𝛼−𝛽) Pruning optimization for the Checkers (Draughts) game.

<img src="resources/game_example.png" width="100%">

## Implementations

This repository contains two implementations:

### Python Implementation
The original Python implementation located in `alpha-beta-pruning-minmax-checkers/` directory.

**Features:**
- Object-oriented design
- Interactive gameplay
- Dynamic depth adjustment
- Multiple evaluation strategies

**Running:**
```bash
cd alpha-beta-pruning-minmax-checkers
python game.py
```

### C Implementation
A high-performance C implementation with modern architecture located in `c-implementation/` directory.

**Key Features:**
- **1D Array Board Representation**: Flexible board with configurable width and height
- **Decoupled Architecture**: AI algorithms completely separated from game logic
- **Modular Design**: Clean separation between board, AI, input, and output modules
- **Evaluation Functions**: Pluggable evaluation strategies (standard and endgame)
- **Performance**: Significantly faster than Python version
- **Standard C99**: Portable and minimal dependencies

**Building and Running:**
```bash
cd c-implementation
make
./checkers
```

See [c-implementation/README.md](c-implementation/README.md) for detailed documentation.

## Algorithm

Both implementations use the Minimax algorithm with Alpha-Beta pruning:
- **Minimax**: Game tree search algorithm for zero-sum games
- **Alpha-Beta Pruning**: Optimization that eliminates branches that won't affect the final decision
- **Dynamic Depth**: Adjusts search depth based on game complexity and computation time
- **Evaluation Functions**: Heuristics for position assessment

## Game Rules

- Standard checkers/draughts rules
- Optional forced captures
- Piece promotion to kings
- Win by capturing all opponent pieces or blocking all moves
