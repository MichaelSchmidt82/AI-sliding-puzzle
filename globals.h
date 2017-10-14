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

/* Cross Application Functions */
template <typename T>
inline void quick_xchng(T & t1, T & t2) {
	t1 ^= t2;
	t2 ^= t1;
	t1 ^= t2;
}
#endif // !GLOBALS_H