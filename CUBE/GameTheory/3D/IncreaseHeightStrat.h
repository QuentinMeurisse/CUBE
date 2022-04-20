#ifndef CUBE_INCREASEHEIGHTSTRAT_H
#define CUBE_INCREASEHEIGHTSTRAT_H

#include "Strategy.h"

class IncreaseHeightStrat : public Strategy {
public:
    IncreaseHeightStrat(int const& idnum, double const& floor);

    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

protected:
    [[nodiscard]] bool equal(const Strategy &other) const override;

protected:
    void print(std::ostream &os) const override;


private:
    int idnum;
    double floor;

};


#endif //CUBE_INCREASEHEIGHTSTRAT_H
