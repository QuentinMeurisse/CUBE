#include "SwapColorStrat.h"
#include "../GraphGame.h"

#include <utility>
#include <memory>
#include <iostream>

using namespace std;

SwapColorStrat::SwapColorStrat(uint64_t sourceNode, uint64_t targetNode)
        : sourceNode(sourceNode), targetNode(targetNode) {}

void SwapColorStrat::select(shared_ptr<Game> g) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(g);
    string sourceColor = gg->getNodeColor(sourceNode);
    string targetColor = gg->getNodeColor(targetNode);
    gg->setNodeColor(sourceNode, targetColor);
    gg->setNodeColor(targetNode, sourceColor);
    shared_ptr<SwapColorStrat> s = make_shared<SwapColorStrat>(*this);
    gg->updateAll(s);
}

void SwapColorStrat::unselect(shared_ptr<Game> g) {
    shared_ptr<GraphGame> gg = dynamic_pointer_cast<GraphGame>(g);
    string sourceColor = gg->getNodeColor(sourceNode);
    string targetColor = gg->getNodeColor(targetNode);
    gg->setNodeColor(targetNode, sourceColor);
    gg->setNodeColor(sourceNode, targetColor);
    shared_ptr<SwapColorStrat> s = make_shared<SwapColorStrat>(*this);
    gg->undoAll(s);
}

void SwapColorStrat::print(ostream &os) const {
    os << "Move color "<< " from " << sourceNode << " to " << targetNode;
}

uint64_t SwapColorStrat::getSourceNode() const {
    return sourceNode;
}

uint64_t SwapColorStrat::getTargetNode() const {
    return targetNode;
}

bool SwapColorStrat::equal(const Strategy &other) const {
    try {
        const auto& scs = dynamic_cast<const SwapColorStrat&>(other);
        return this->sourceNode == scs.sourceNode and this->targetNode == scs.targetNode;
    } catch (...) {
        return false;
    }
}
