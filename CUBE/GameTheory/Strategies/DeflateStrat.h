#ifndef CUBE_DEFLATESTRAT_H
#define CUBE_DEFLATESTRAT_H

#include "Strategy.h"
#include "ChangeColorStrat.h"

#include <deque>
#include <string>

class DeflateStrat : public Strategy {

public:
    DeflateStrat(int idnum, std::string color);

    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

protected:
    void print(std::ostream &os) const override;

    [[nodiscard]] bool equal(const Strategy &other) const override;

private:
    int idnum;
    std::deque<std::shared_ptr<ChangeColorStrat>> q;
    std::string color; //Same use as InflateStrat

};


#endif //CUBE_DEFLATESTRAT_H
