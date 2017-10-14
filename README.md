# AI-sliding-puzzle
Solve an NxN sliding puzzle problem using the A* algorithm

### How to Build:
make

### Usage:
Generate a board:  `./random_board <SEED> <NUM_OF_SHUFFLES> > <START_STATE_FILE>`
  
### Solve a board:
`./a-star <MODE> < <START_STATE_FILE>`

where:

Mode 0: Use no heuristic (Same as greedy search)
Mode 1: Use number of tiles displaced.
Mode 2: Use Sum of manhattan distances among tiles.
Mode 3: Use Sum of euclidean distances among tiles.
