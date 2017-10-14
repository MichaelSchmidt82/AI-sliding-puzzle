#include "state.h"

/* constructors */
State::State (Mode _mode)
	: board(nullptr), parent(nullptr), depth(0) {
		set_mode(_mode);
};

State::State (const State & original) {
	this->n = original.n;
	this->depth = original.depth;
    try {
    	board = new Tile*[n];
    	for (int i = 0; i < n; i++)
    		board[i] = new Tile[n];
    } catch (bad_alloc & ba) {
        cerr << "bad allocation: " << ba.what() << endl;
    }

	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			this->board[r][c] = original.board[r][c];

	this->blank_spot = original.blank_spot;
	set_mode(original.mode);
}

State::State (const StatePtr original) {
	this->n = original->n;
	this->depth = original->depth;
    try {
    	board = new Tile*[n];
    	for (int i = 0; i < n; i++)
    		board[i] = new Tile[n];
    } catch (bad_alloc & ba) {
        cerr << "bad allocation: " << ba.what() << endl;
    }
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			this->board[r][c] = original->board[r][c];

	this->blank_spot = original->blank_spot;
	set_mode(original->mode);
}

State::State (Board _board, int _n, Mode _mode) : depth(0), n(_n), mode(_mode), parent(nullptr) {
    try {
        board = new Tile*[n];
        for (int i = 0; i < n; i++)
        	board[i] = new Tile[n];
    } catch (bad_alloc & ba) {
        cerr << "bad allocation: " << ba.what() << endl;
    }

	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++) {
			board[r][c] = _board[r][c];
			if (board[r][c] == 0)
				blank_spot = Pair(r, c);
		}

	set_mode(_mode);
}

/* Destructor */
State::~State () {
	if (board)
		delete[] board;
	board = nullptr;
}

/* operator overloads */
bool State::operator< (const State & rhs) const {
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			if (this->board[r][c] != rhs.board[r][c])
				return (this->board[r][c] < rhs.board[r][c]);

	return false;
}

bool State::operator== (const State & rhs) const {
	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++)
			if (this->board[r][c] != rhs.board[r][c])
				return false;

	return true;
}

/* Observers */
void State::print_board () const {
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++)
			cout << board[r][c] << ' ';
		cout << endl;
	}
}

bool State::is_goal () const {
	if (blank_spot.r != 0 || blank_spot.c != 0)
		return false;

	for (int r = 0, i = 0; r < n; r++)
		for (int c = 0; c < n; c++, i++)
			if (board[r][c] != i)
				return false;

	return true;
}

//////////////////////
/* Board operations */
//////////////////////

bool State::move_up () {
	if (blank_spot.r == 0)
		return false;

	::quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r - 1][blank_spot.c]);
	blank_spot.r -= 1;
	return true;
}

bool State::move_down () {
	if (blank_spot.r == n - 1)
		return false;

	::quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r + 1][blank_spot.c]);
	blank_spot.r += 1;
	return true;
}

bool State::move_right () {
	if (blank_spot.c == n - 1)
		return false;

	::quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r][blank_spot.c + 1]);
	blank_spot.c += 1;
	return true;
}

bool State::move_left () {
	if (blank_spot.c == 0)
		return false;

	::quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r][blank_spot.c - 1]);
	blank_spot.c -= 1;
	return true;
}

void State::set_mode (Mode _mode) {
	switch (_mode) {
		case 0:
			calculate_fn = &State::zero_hn;
			break;
		case 1:
			calculate_fn = &State::count_displaced;
			break;
		case 2:
			calculate_fn = &State::sum_of_manhattan;
			break;
		case 3:
			calculate_fn = &State::custom_h;
			break;
		default:
			calculate_fn = &State::sum_of_manhattan;
			break;
	}
	mode = _mode;
}

/////////////////////////
/* heuristic functions */
/////////////////////////

int State::count_displaced () const {
	int count = 0;
	for (int r = 0, i = 0; r < n; r++)
		for (int c = 0; c < n; c++, i++)
			if (board[r][c] != i && i != 0)
				count++;

	return count;
}

int State::sum_of_manhattan () const {
	Pair start_coord, end_coord;
	int hn = 0;

	for (int r = 0, i = 0; r < n; r++)
		for (int c = 0; c < n; c++, i++) {
            /* don't count the blank_spot (i != 0) */
			if (board[r][c] != i && i != 0) {
				end_coord = Pair(r, c);

				for (int sr = n - 1; sr >= 0; sr--)
					for (int sc = n - 1; sc >= 0; sc--)
						if (board[sr][sc] == i) {
							start_coord = Pair(sr, sc);
							break;
						}
                hn += abs(end_coord.r - start_coord.r) + abs(end_coord.c - start_coord.c);
            }
		}
	return hn;
}

/* use distance formula */
int State::custom_h () const {
	Pair end_coord, start_coord;
	double distance = 0;

	for (int r = 0, i = 0; r < n; r++)
		for (int c = 0; c < n; c++, i++)
			if (board[r][c] != i && i != 0) {
				end_coord = Pair(r, c);

				for (int sr = n - 1; sr >= 0; sr--)
					for (int sc = n - 1; sc >= 0; sc--)
						if (board[sr][sc] == i) {
							start_coord = Pair(sr, sc);
							break;
						}
				distance += sqrt(pow(end_coord.r - start_coord.r, 2) + pow(end_coord.c - start_coord.c, 2));
			}
	return floor(distance);
}
