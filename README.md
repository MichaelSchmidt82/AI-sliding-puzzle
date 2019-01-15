# AI-sliding-puzzle
Solve an NxN sliding puzzle problem using the A* algorithm

### How to Build:
clone the project and type `make`

### Usage:
Generate a board:  `./random_board <SEED> <NUM_OF_SHUFFLES> < <COMPLETED_STATE>`  
Solve a board:  `./a_star <MODE> < <START_STATE_FILE>`  
  
or simply chain the operations together:  
`./random_board 1 100 < puzzle.txt | ./a-star 2`  

#### where:  
`<SEED>`  
A seed value for the random number generator.  

`<NUM_OF_SHUFFLES>`  
Specifies show many shuffles to make on the board.

`<COMPLETED_STATE>`  
A file that specifies what a solution should look like.  Use 0 for blank.    
  
e.x.:  
0 1 2  
3 4 5  
6 7 8  

`<MODE>`    
Mode 0: Use no heuristic (Same as greedy search).  
Mode 1: Use number of tiles displaced.  
Mode 2: Use Sum of manhattan distances among tiles.  
Mode 3: Use Sum of euclidean distances among tiles.
  
`<START_STATE_FILE>`  
Specify via STDIN the puzzle.  As noted earlier, use `./random_board` to do this.  
  
`./random_board 1 100 < completed.txt > shuffled_puzzle.txt` then `./a-star 2 < shuffled_puzzle.txt`  
or simply...  
`./random_board 1 100 < puzzle.txt | ./a_star 2`

