#include "state.h"

/* Constructors */
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
				blank_spot = make_pair(r, c);
		}

	set_mode(_mode);
}

/* Destructor */
State::~State () {
	if (board) {
		for (int i = 0; i < n; i++)
			delete [] board[i];
		delete [] board;
	}
	board = nullptr;
}

/* Operators */
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
            if (board[r][c] == 0)
                cout << "  - ";
            else
			    cout << setw(3) << board[r][c] << ' ';
		cout << endl;
	}
}

bool State::is_goal () const {
	if (blank_spot.first != 0 || blank_spot.second != 0)
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
	if (blank_spot.first == 0)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first - 1][blank_spot.second]);
	blank_spot.first -= 1;
	return true;
}

bool State::move_down () {
	if (blank_spot.first == n - 1)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first + 1][blank_spot.second]);
	blank_spot.first += 1;
	return true;
}

bool State::move_right () {
	if (blank_spot.second == n - 1)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first][blank_spot.second + 1]);
	blank_spot.second += 1;
	return true;
}

bool State::move_left () {
	if (blank_spot.second == 0)
		return false;

	::quick_xchng(board[blank_spot.first][blank_spot.second],
		board[blank_spot.first][blank_spot.second - 1]);
	blank_spot.second -= 1;
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
/* Heuristic functions */
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
	pair<int, int> start_coord, end_coord;
	int hn = 0;

	for (int r = 0, i = 0; r < n; r++)
		for (int c = 0; c < n; c++, i++) {
            /* Must not count the blank_spot (i != 0) */
			if (board[r][c] != i && i != 0) {
				end_coord = make_pair(r, c);

				for (int sr = n - 1; sr >= 0; sr--)
					for (int sc = n - 1; sc >= 0; sc--)
						if (board[sr][sc] == i) {
							start_coord = make_pair(sr, sc);
							break;
						}
                hn += abs(end_coord.first - start_coord.first) +
					abs(end_coord.second - start_coord.second);
            }
		}
	return hn;
}

/* Use euclidean distance formula */
int State::custom_h () const {
	pair<int, int> end_coord, start_coord;
	double distance = 0;

	for (int r = 0, i = 0; r < n; r++)
		for (int c = 0; c < n; c++, i++)
			if (board[r][c] != i && i != 0) {
				end_coord = make_pair(r, c);

				for (int sr = n - 1; sr >= 0; sr--)
					for (int sc = n - 1; sc >= 0; sc--)
						if (board[sr][sc] == i) {
							start_coord = make_pair(sr, sc);
							break;
						}
				distance += sqrt(pow(end_coord.first - start_coord.first, 2) +
					pow(end_coord.second - start_coord.second, 2));
			}
	return floor(distance);
}
