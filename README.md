# AI-sliding-puzzle
Solve an NxN sliding puzzle problem using the A* algorithm

### How to Build:
clone the project and type `make`

### Usage:
Generate a board:  `./random_board <SEED> <NUM_OF_SHUFFLES> < <COMPLETED_STATE>`  
Solve a board:  `./a-star <MODE> < <START_STATE_FILE>`  
  
or simply chain the operations together e.x.:  
`./a-star 2 < ./random_board 1 100 < puzzle.txt`  

#### where:  
`<SEED>`  
A seed value for the random number generator.  

`<NUM_OF_SHUFFLES>`  
Specifies show many shuffles to make on the board.

`<SOLUTION_FILE>`  
Specifies what a solution should look like.  

e.x.:  
0 1 2  
3 4 5  
6 7 8  
where 0 is the blank spot.

`<MODE>`    
Mode 0: Use no heuristic (Same as greedy search).  
Mode 1: Use number of tiles displaced.  
Mode 2: Use Sum of manhattan distances among tiles.  
Mode 3: Use Sum of euclidean distances among tiles.
