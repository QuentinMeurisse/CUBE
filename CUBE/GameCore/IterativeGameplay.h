#ifndef CUBE_ITERATIVEGAMEPLAY_H
#define CUBE_ITERATIVEGAMEPLAY_H


#include <vector>
#include <memory>
#include "Player.h"
#include "Listener.h"

class IterativeGameplay {
public:
    IterativeGameplay(std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Game> game);

    virtual ~IterativeGameplay();

    virtual void play();

    void addListener(const std::shared_ptr<Listener>& l);

    [[nodiscard]] const std::shared_ptr<Game> &getGame() const;

    [[nodiscard]] const std::vector<std::shared_ptr<Player>> &getPlayers() const;

    [[nodiscard]] const std::string &getLastStrat() const;

    void stop();

    virtual std::shared_ptr<Strategy> selectStrat(std::shared_ptr<Player> p) = 0;

    virtual std::vector<std::shared_ptr<Player>> orderPLayers() = 0;

protected:
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Game> game;
    std::vector<std::shared_ptr<Listener>> listeners{};
    std::string lastStrat{}; // used for debug
    bool stopFlag = false;


};


#endif //CUBE_ITERATIVEGAMEPLAY_H
