#ifndef PAIR_H
#define PAIR_H

#include "globals.h"

/* A simple struct to hold coordinate points */
struct Pair {

	/* Constructors */
	Pair () : r(0), c(0) {}
	Pair (int _r, int _c) : r(_r), c(_c) {}
	Pair (const Pair & p) : r(p.r), c(p.c) {}
	Pair (PairPtr p) : r(p->r), c(p->c) {}

	/* Operators */
	void operator= (const Pair & p);
	bool operator== (const Pair & rhs);
	bool operator!= (const Pair & rhs);

	/* Members */
	int r;
	int c;
};
#endif // !PAIR_H
