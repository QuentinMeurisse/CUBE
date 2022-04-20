#ifndef CUBE_INFLATESTRAT_H
#define CUBE_INFLATESTRAT_H

#include "Strategy.h"
#include "ChangeColorStrat.h"
#include "../TwoRadiusConstraint.h"

#include <deque>
#include <string>

class InflateStrat : public Strategy {

public:
    InflateStrat(int const &idnum, std::string color, unsigned int const &rb, unsigned int const &rg);

    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

protected:
    void print(std::ostream &os) const override;

    [[nodiscard]] bool equal(const Strategy &other) const override;

private:
    int idnum;
    std::deque<std::shared_ptr<Strategy>> q;
    TwoRadiusConstraint constraint  = TwoRadiusConstraint(0, 0);
    std::string color;
    //Know this variable allow to determine easily on which type of component this strategy will have an effect
};


#endif //CUBE_INFLATESTRAT_H
