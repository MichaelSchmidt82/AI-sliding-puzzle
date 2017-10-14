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

#endif
