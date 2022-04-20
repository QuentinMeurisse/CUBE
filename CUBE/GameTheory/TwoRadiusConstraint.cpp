#include "TwoRadiusConstraint.h"
#include "GameTheoryUtility.h"

#include <map>
#include <vector>
#include <iostream>

using namespace std;

TwoRadiusConstraint::TwoRadiusConstraint(unsigned int buildRadius, unsigned int greenRadius) : buildRadius(buildRadius),
                                                                                               greenRadius(
                                                                                                       greenRadius) {}

bool TwoRadiusConstraint::validChangeColor(const std::shared_ptr<UrbanBlockGame>& game,
                                           const std::shared_ptr<ChangeColorStrat> &strat) {

    if (strat->getNewColor() == "#00f000")
        return validChangeColorOnGreenSpace(game, strat);
    else
        return validChangeColorOnBuilding(game, strat);
}

bool TwoRadiusConstraint::validChangeColorOnGreenSpace(const std::shared_ptr<UrbanBlockGame> &game,
                                                       const std::shared_ptr<ChangeColorStrat> &strat) const {

    if(greenRadius == 0)
        return true;
    else {
        shared_ptr<Tiling> t = game->getT();
        strat->select(game);

        deque<unsigned int> ball = GameTheoryUtility::faceBall(t->getCells(), strat->getK(), greenRadius);
        unsigned int targetComponent = t->getComponent(strat->getK());
        for (unsigned int x : ball) {
            if (t->getComponent(x) != targetComponent and t->getColor(x) != "#ffffff"){
                strat->unselect(game);
                return false;
            }
        }
        strat->unselect(game);
        return true;
    }
}

bool TwoRadiusConstraint::validChangeColorOnBuilding(const std::shared_ptr<UrbanBlockGame> &game,
                                                     const std::shared_ptr<ChangeColorStrat> &strat) const {

    if (buildRadius == 0 and greenRadius == 0)
        return true;
    else{
        shared_ptr<Tiling> t = game->getT();
        strat->select(game);

        map<unsigned int, deque<unsigned int>> dist = GameTheoryUtility::faceDistances(t->getCells(), strat->getK(), buildRadius);
        unsigned int targetComponent = t->getComponent(strat->getK());
        for(auto const& x : dist){
            for (unsigned int const& k : x.second){
                if((t->getColor(k) == "#00f000" and x.first <= greenRadius)
                    or (t->getComponent(k) != targetComponent and t->getColor(k) != "#ffffff" and t->getColor(k) != "#00f000")) {

                    strat->unselect(game);
                    return false;
                }
            }
        }
        strat->unselect(game);
        return true;
    }
}
