#ifndef CUBE_SWAPCOLORSTRAT_H
#define CUBE_SWAPCOLORSTRAT_H

#include "Strategy.h"

#include <string>

class SwapColorStrat : public Strategy {
public:
    SwapColorStrat(uint64_t sourceNode, uint64_t targetNode);

    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

    [[nodiscard]] uint64_t getSourceNode() const;

    [[nodiscard]] uint64_t getTargetNode() const;

protected:
    void print(std::ostream &os) const override;

    [[nodiscard]] bool equal(const Strategy &other) const override;


private:
    uint64_t sourceNode;
    uint64_t targetNode;
};


#endif //CUBE_SWAPCOLORSTRAT_H
