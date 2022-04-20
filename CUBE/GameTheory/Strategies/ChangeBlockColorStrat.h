#ifndef CUBE_CHANGEBLOCKCOLORSTRAT_H
#define CUBE_CHANGEBLOCKCOLORSTRAT_H


#include "Strategy.h"
#include "ChangeColorStrat.h"

#include <deque>
#include <string>

class ChangeBlockColorStrat : public Strategy{
public:
    ChangeBlockColorStrat(unsigned int const&  k, std::string newColor);

    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

protected:
    void print(std::ostream &os) const override;

    [[nodiscard]] bool equal(const Strategy &other) const override;

private:
    unsigned int k;
    std::string newColor;
    std::deque<std::shared_ptr<ChangeColorStrat>> q;

};


#endif //CUBE_CHANGEBLOCKCOLORSTRAT_H
