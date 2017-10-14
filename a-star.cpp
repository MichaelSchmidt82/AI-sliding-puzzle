/*
Author:         Michael Schmidt
Class:			CSCI 4350 - Intro to AI
Instructor:	    Dr. Joshua Phillips
Due:            28 Sept 2017

Description:    a-star implementation

Assumptions:    Board is N x N.
*/

//#include "globals.h"
#ifndef GLOBALS_H
#define GLOBALS_H

/* Includes */
#include <ctime>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stack>
#include <set>
#include <vector>

/* Using Statements */
using std::atoi;
using std::bad_alloc;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::pow;
using std::set;
using std::sqrt;
using std::stack;
using std::vector;

/* Forward Declarations */
class State;
struct Pair;
struct ClosedPtrCompare;

/* Typedefs */
typedef State* StatePtr;
typedef Pair* PairPtr;
typedef int Tile;		// unsigned char may save space
typedef Tile* TilePtr;
typedef Tile** Board;
typedef stack<StatePtr> Path;
typedef int (State::*HeuristicCallback) (void) const;

/* Enums */
enum Mode {
	ZERO,
	DISPLACE,
	SUM_OF_MAN,
	CUSTOM
};

/* Structs */

struct Pair {
    /* A simple struct to hold coordinate points */
	/* Constructors */
	Pair () : r(0), c(0) {}
	Pair (int _r, int _c) : r(_r), c(_c) {}
	Pair (const Pair & p) : r(p.r), c(p.c) {}
	Pair (PairPtr p) : r(p->r), c(p->c) {}

	/* Operator overloads */
	void operator= (const Pair & p) {
		this->r = p.r;
		this->c = p.c;
	}
	bool operator== (const Pair & rhs) { return (this->r == rhs.r) && (this->c == rhs.c); }
	bool operator!= (const Pair & rhs) { return (this->r != rhs.r) || (this->c != rhs.c); }

	/* Members */
	int r;
	int c;
};

/* Classes */
class State {
public:
	/* Constructors & Decstructors*/
	State (Mode _mode);
	State (Board _board, int _n, Mode _mode);
	State (const State & original);
	State (const StatePtr original);
	~State ();

	/* Operators */
	bool operator== (const State & rhs) const;
	bool operator!= (const State & rhs) const { return !(*this == rhs); }
	bool operator< (const State & rhs) const;

	/* Observers */
	void print_board () const;
	bool is_goal () const;
	int get_depth () const { return depth; }
	int get_id () const { return id; }
	int fn () const { return depth + (this->*(this->calculate_fn))(); }
	StatePtr get_parent () const { return parent; }

	/* Mutators */
	void set_id (int _id) { this->id = _id; }
	void shuffle_board (int seed, int iterations);
	void attach_parent (StatePtr parent) { this->parent = parent; this->depth = parent->depth + 1; }
	void remove_parent () { this->parent = nullptr; }

	/* Board Operations */
	bool move_up ();
	bool move_down ();
	bool move_left ();
	bool move_right ();

private:
	int n;				// Side length of puzzle
	int id;
	int depth;

	Board board;
	Mode mode;
	Pair blank_spot;
	StatePtr parent;

	HeuristicCallback calculate_fn;

	/* Helpers*/
	void set_mode (Mode mode);

	/* heuristic cost calculators */
	int zero_hn () const { return 0; }
	int count_displaced () const;
	int sum_of_manhattan () const;
	int custom_h () const;
};

/* Pointer Comparers */
struct ClosedPtrCompare {
	bool operator()(const StatePtr lhs, const StatePtr rhs) const { return *lhs < *rhs; }
};

struct FrontierPtrCompare {
	bool operator()(const StatePtr lhs, const StatePtr rhs) const {
		return (lhs->fn() == rhs->fn()) ? *lhs < *rhs : lhs->fn() < rhs->fn();
	}
};

/* Cross app Functions */
template <typename T>
inline void quick_xchng(T & t1, T & t2) {
	t1 ^= t2;
	t2 ^= t1;
	t1 ^= t2;
}
#endif // !GLOBALS_H

/* Prototypes */
void expand_state (StatePtr parent,
    set<StatePtr, ClosedPtrCompare> & closed_list,
    set<StatePtr, FrontierPtrCompare> & frontier);
void init_board(Board & board, size_t & side);

//////////////
/* INT MAIN */
//////////////

int main (int argc, char *argv[]) {
	/* Statistics */
	size_t v = 0;
	size_t n;
	size_t d;
	double b;

	/* Variables */
    size_t side; // side length of puzzle
    Path path;
    Board board;
	StatePtr curr_state;
	set<StatePtr, ClosedPtrCompare> closed_list;
	set<StatePtr, FrontierPtrCompare> frontier;

    /* Create initial state */
    init_board(board, side);
    try {
        curr_state = new State(board, side, Mode(atoi(argv[1])));
    } catch (bad_alloc & ba) {
        cerr << "bad allocation: " << ba.what() << endl;
    }
    /* Perform a-star search */
	while (!curr_state->is_goal()) {
		closed_list.insert(curr_state);
		expand_state(curr_state, closed_list, frontier);

		curr_state = *frontier.begin();
		frontier.erase(frontier.begin());
		v++;
	}

	/* Compute Stats */
	n = frontier.size() + closed_list.size();
	d = curr_state->get_depth();
	b = pow(n, 1 / double(d));

	/* Print results */
	cout << "V=" << v << endl;
	cout << "N=" << n << endl;
	cout << "d=" << d << endl;
	cout << "b=" << b << endl;
	cout << endl;

    /* Push states onto a stack for path */
	while (curr_state != nullptr) {
		path.push(curr_state);
		curr_state = curr_state->get_parent();
	}

    /* Print path/state history */
    while (!path.empty()) {
		curr_state = path.top();
		curr_state->print_board();
		cout << endl;
		path.pop();
	}

	for (StatePtr p : frontier)
		if (p != nullptr) {
			delete p;
			p = nullptr;
		}

	for (StatePtr p : closed_list)
		if (p != nullptr) {
			delete p;
			p = nullptr;
		}

  /* Exit */
	return 0;
}

///////////////
/* Functions */
///////////////

void expand_state (StatePtr parent, set<StatePtr, ClosedPtrCompare> & closed_list, set<StatePtr, FrontierPtrCompare> & frontier) {
	static int id = 0;
	StatePtr expand[4];

	/* Generate new states */
	for (int i = 0; i < 4; i++) {
        try {
            expand[i] = new State(parent);
        } catch (bad_alloc& ba) {
            cerr << "bad allocation: " << ba.what() << endl;
        }
    }

    /* Create new states, remove ones that fail */
	if (!expand[0]->move_down()) {
		delete expand[0];
		expand[0] = nullptr;
	}

	if (!expand[1]->move_left()) {
		delete expand[1];
		expand[1] = nullptr;
	}

	if (!expand[2]->move_up()) {
		delete expand[2];
		expand[2] = nullptr;
	}

	if (!expand[3]->move_right()) {
		delete expand[3];
		expand[3] = nullptr;
	}

	/* give each state an id */
	for (int i = 0; i < 4; i++)
		if (expand[i] != nullptr) {
			expand[i]->set_id(id);
			id++;
		}

	/* check created states against known (closed) states */
	for (int i = 0; i < 4; i++)
		if (expand[i] != nullptr) {
			auto find = closed_list.find(expand[i]);
			if (find == closed_list.end()) {
				expand[i]->attach_parent(parent);
				frontier.insert(expand[i]);
			}
		}
}

/* Create board using stdin */
void init_board (Board & board, size_t & side) {
    Tile tile;
    vector<Tile> parse;

    while (cin >> tile)
        parse.push_back(tile);

    assert(ceil(sqrt(parse.size())) == sqrt(parse.size()) && "Assumes NxN puzzle.");
    side = sqrt(parse.size());

    try {
        board = new TilePtr[side];

    	for (int r = 0, i = 0; r < side; r++) {
            board[r] = new Tile[side];
            for (int c = 0; c < side; c++, i++)
                board[r][c] = parse[i];
        }
    } catch (bad_alloc & ba) {
        cerr << "bad allocation: " << ba.what() << endl;
    }
}

/////////////////////////////////
/* State object implementation */
/////////////////////////////////

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
