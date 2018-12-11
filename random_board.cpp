/*
Author:         Michael Schmidt
Class:			CSCI 4350 - Intro to Artificial Intelligence
Instructor:	    Dr. Joshua Phillips
Due:            28 Sept 2017

Description:    Generate a random (and valid) puzzle to solve

Assumptions:    Board is N x N.
*/

#include "globals.h"

inline bool move_up(Board & board, pair<int, int>& blank_spot);
inline bool move_down(Board & board, pair<int, int>& blank_spot, int n);
inline bool move_left(Board & board, pair<int, int>& blank_spot);
inline bool move_right(Board & board, pair<int, int>& blank_spot, int n);

void shuffle_board(Board & board, pair<int, int>& blank_spot, int n, int seed, int iterations);

int main (int argc, char *argv[]) {

	assert(argc == 3 && "Provide a random seed, iterations as arguements.");

	int side, num;
    pair<int, int> blank_spot;
    Board board;
    vector<Tile> parse;

  	while (cin >> num)
  		parse.push_back(num);

    assert(ceil(sqrt(parse.size())) == sqrt(parse.size()) && "Program assumes NxN puzzle");
  	side = sqrt(parse.size());

  	board = new Tile*[side];
  	for (int i = 0; i < side; i++)
  		board[i] = new Tile[side];

  	for (int r = 0, i = 0; r < side; r++)
  		for (int c = 0; c < side; c++, i++)
  			board[r][c] = parse[i];

    shuffle_board(board, blank_spot, side, atoi(argv[1]), atoi(argv[2]));

	/* print shuffled board to stdout */
    for (int r = 0; r < side; r++) {
      for (int c = 0; c < side; c++)
        cout << board[r][c] << ' ';
      cout << endl;
    }

	/* Cleanup heap allocations */
	for (int i = 0; i < side; i++)
        delete [] board[i];
    delete [] board;

	return 0;
}

void shuffle_board (Board & board, pair<int, int>& blank_spot, int n, int seed, int iterations) {
	bool success;
	int rand_num;

	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++) {
			if (board[r][c] == 0)
				blank_spot = make_pair(r, c);
                break;
            }

	srand(seed);

	for (int i = 0; i < iterations; i++) {
		rand_num = rand() % 4;

		switch (rand_num) {
		case 0:
			success = move_up(board, blank_spot);
			break;
		case 1:
			success = move_down(board, blank_spot, n);
			break;
		case 2:
			success = move_left(board, blank_spot);
			break;
		case 3:
			success = move_right(board, blank_spot, n);
			break;
		}

        /* If move was unsuccessful, try and try again */
		if (!success)
			iterations++;
	}
}

inline bool move_up (Board & board, pair<int, int>& blank_spot) {
	if (blank_spot.first == 0)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first - 1][blank_spot.second]);
	blank_spot.first -= 1;
	return true;
}

inline bool move_down (Board & board, pair<int, int>& blank_spot, int n) {
	if (blank_spot.first == n - 1)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first + 1][blank_spot.second]);
	blank_spot.first += 1;
	return true;
}

inline bool move_right (Board & board, pair<int, int>& blank_spot, int n) {
	if (blank_spot.second == n - 1)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first][blank_spot.second + 1]);
	blank_spot.second += 1;
	return true;
}

inline bool move_left (Board & board, pair<int, int>& blank_spot) {
	if (blank_spot.second == 0)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first][blank_spot.second - 1]);
	blank_spot.second -= 1;
	return true;
}
