#include "ColorNodeInvariant.h"
#include "../Strats/SwapColorStrat.h"

#include <memory>

using namespace std;

ColorNodeInvariant::ColorNodeInvariant(const shared_ptr<GraphGame>& gg) : Invariant(gg) {

}

void ColorNodeInvariant::update(const std::shared_ptr<Strategy> &strat) {
    shared_ptr<SwapColorStrat> msc = dynamic_pointer_cast<SwapColorStrat>(strat);
    auto it = find(tokens.begin(), tokens.end(), msc->getSourceNode());
    if (it != tokens.end()){
        auto index = distance(tokens.begin(), it);
        tokens[index] = msc->getTargetNode();
    }

}

void ColorNodeInvariant::undo(const std::shared_ptr<Strategy> &strat) {
    shared_ptr<SwapColorStrat> msc = dynamic_pointer_cast<SwapColorStrat>(strat);
    auto it = find(tokens.begin(), tokens.end(), msc->getTargetNode());
    if (it != tokens.end()) {
        auto index = distance(tokens.begin(), it);
        tokens[index] = msc->getSourceNode();
    }
}

const vector<uint64_t> & ColorNodeInvariant::getTokens() const {
    return tokens;
}

void ColorNodeInvariant::addNode(uint64_t n) {
    tokens.push_back(n);
}

size_t ColorNodeInvariant::numTokens() {
    return tokens.size();
}



