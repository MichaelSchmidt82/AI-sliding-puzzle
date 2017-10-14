#ifndef STATE_H
#define STATE_H

#include "globals.h"
#include "pair.h"

class State {
public:
	/* Constructors & Decstructors */
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
	int fn () const { return depth + (this->*(this->calculate_fn))(); }
	StatePtr get_parent () const { return parent; }

	/* Mutators */
	void attach_parent (StatePtr parent) { this->parent = parent; this->depth = parent->depth + 1; }
	void remove_parent () { this->parent = nullptr; }

	/* Board Operations */
	bool move_up ();
	bool move_down ();
	bool move_left ();
	bool move_right ();

private:
	int n;				// Side length of puzzle
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

#endif
