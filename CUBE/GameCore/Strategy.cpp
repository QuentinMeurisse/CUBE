#include "Strategy.h"

Strategy::~Strategy() = default;

std::ostream &operator<<(std::ostream &os, const Strategy &strategy) {
    strategy.print(os);
    return os;
}

bool Strategy::operator==(const Strategy &other) const {
    return equal(other);
}
