#include "Invariant.h"

#include <utility>

Invariant::Invariant(std::shared_ptr<Game> g) : g(std::move(g)) {}


Invariant::Invariant(Invariant const& other) {
    g = other.g;
}

Invariant &Invariant::operator=(Invariant const& other) {
    if(this != &other){
        g = other.g;
    }
    return *this;
}


