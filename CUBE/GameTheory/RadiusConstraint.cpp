#include "RadiusConstraint.h"
#include "GameTheoryUtility.h"

using namespace std;

RadiusConstraint::RadiusConstraint(unsigned int radius) : radius(radius) {}

bool RadiusConstraint::validChangeColor(const std::shared_ptr<UrbanBlockGame> &game,
                                        const std::shared_ptr<ChangeColorStrat>& strat) {
    if(radius == 0)
        return true;
    else {
        shared_ptr<Tiling> t = game->getT();
        strat->select(game);

        deque<unsigned int> ball = GameTheoryUtility::faceBall(t->getCells(), strat->getK(), radius);
        unsigned int targetComponent = t->getComponent(strat->getK());
        for (unsigned int x : ball) {
            if (t->getComponent(x) != targetComponent && t->getColor(x) != "#ffffff"){
                strat->unselect(game);
                return false;
            }
        }
        strat->unselect(game);
        return true;
    }
}
