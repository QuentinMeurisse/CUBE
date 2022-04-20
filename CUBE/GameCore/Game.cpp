#include "Game.h"
#include <iostream>

Game::~Game() = default;

std::ostream &operator<<(std::ostream &os, const Game &game) {
    std::cout << "Game" << std::endl;
    return os;
}
