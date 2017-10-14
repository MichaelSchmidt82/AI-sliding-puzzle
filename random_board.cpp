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
/* A simple struct to hold coordinate points */
struct Pair {
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

inline bool move_up(Board & board, Pair & blank_spot);
inline bool move_down(Board & board, Pair & blank_spot, int n);
inline bool move_left(Board & board, Pair & blank_spot);
inline bool move_right(Board & board, Pair & blank_spot, int n);

void shuffle_board(Board & board, Pair & blank_spot, int n, int seed, int iterations);

int main (int argc, char *argv[]) {
  	int side, num;
    Pair blank_spot;
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

    for (int r = 0; r < side; r++) {
      for (int c = 0; c < side; c++)
        cout << board[r][c] << ' ';
      cout << endl;
    }
}

void shuffle_board (Board & board, Pair & blank_spot, int n, int seed, int iterations) {
	bool success;
	int rand_num;

	for (int r = 0; r < n; r++)
		for (int c = 0; c < n; c++) {
			if (board[r][c] == 0)
				blank_spot = Pair(r, c);
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

        /* If move was unsuccessful, try again */
		if (!success)
			iterations++;
	}
}

inline bool move_up (Board & board, Pair & blank_spot) {
	if (blank_spot.r == 0)
		return false;

	quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r - 1][blank_spot.c]);
	blank_spot.r -= 1;
	return true;
}

inline bool move_down (Board & board, Pair & blank_spot, int n) {
	if (blank_spot.r == n - 1)
		return false;

	quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r + 1][blank_spot.c]);
	blank_spot.r += 1;
	return true;
}

inline bool move_right (Board & board, Pair & blank_spot, int n) {
	if (blank_spot.c == n - 1)
		return false;

	quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r][blank_spot.c + 1]);
	blank_spot.c += 1;
	return true;
}

inline bool move_left (Board & board, Pair & blank_spot) {
	if (blank_spot.c == 0)
		return false;

	quick_xchng(board[blank_spot.r][blank_spot.c], board[blank_spot.r][blank_spot.c - 1]);
	blank_spot.c -= 1;
	return true;
}
