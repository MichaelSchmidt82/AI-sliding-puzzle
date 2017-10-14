#include "pair.h"

void Pair::operator= (const Pair & p) {
    this->r = p.r;
    this->c = p.c;
}

bool Pair::operator== (const Pair & rhs) {
    return (this->r == rhs.r) && (this->c == rhs.c);
}

bool Pair::operator!= (const Pair & rhs) {
    return (this->r != rhs.r) || (this->c != rhs.c);
}
