#ifndef CUBE_GAME_H
#define CUBE_GAME_H


#include <ostream>

class Game {
public:
    virtual ~Game();

    friend std::ostream &operator<<(std::ostream &os, const Game &game);
};


#endif //CUBE_GAME_H
