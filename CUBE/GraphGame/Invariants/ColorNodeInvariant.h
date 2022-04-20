#ifndef CUBE_COLORNODEINVARIANT_H
#define CUBE_COLORNODEINVARIANT_H

#include "Invariant.h"
#include "../GraphGame.h"

#include <vector>

/**
 * Maintains a list of token enumerated from 0 such that tokens[i] is the number of the colored node
 */
class ColorNodeInvariant : public Invariant{

public:

    explicit ColorNodeInvariant(const std::shared_ptr<GraphGame>& gg);

    void update(const std::shared_ptr<Strategy> &strat) override;

    void undo(const std::shared_ptr<Strategy> &strat) override;

    [[nodiscard]] const std::vector<uint64_t> & getTokens() const;

    void addNode(uint64_t n);

    size_t numTokens();

private:
    std::vector<uint64_t> tokens;
};


#endif //CUBE_COLORNODEINVARIANT_H
