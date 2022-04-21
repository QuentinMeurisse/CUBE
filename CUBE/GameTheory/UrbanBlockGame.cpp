#include "UrbanBlockGame.h"
#include "Invariants/BuildingInvariant.h"
#include "Invariants/GreenSpaceInvariant.h"

#include <utility>
#include <map>
#include <algorithm>
#include <vector>
#include <iostream>


using namespace std;

UrbanBlockGame::UrbanBlockGame(shared_ptr<Tiling> t) : t(move(t)) {}

void UrbanBlockGame::addInvariant(shared_ptr<Invariant> inv, char const &key) {
    invariants[key] = move(inv);
}

const shared_ptr<Tiling> &UrbanBlockGame::getT() const {
    return t;
}

void UrbanBlockGame::updateAll(const shared_ptr<Strategy> &s) {
    for(const auto& inv : invariants){
        inv.second->update(s);
    }
}

void UrbanBlockGame::undoAll(const shared_ptr<Strategy> &s) {
    for(const auto& inv : invariants){
        inv.second->undo(s);
    }
}

optional<shared_ptr<GroupOfCells>> UrbanBlockGame::getGroupOfCells(const unsigned int &k) {
    string color = t->getColor(k);
    char key = color == "#f00000" ? 'b' : color == "#00f000" ? 'g' : 0;
    if (key != 0) {
        shared_ptr<UbranBlockGameInvariant> inv = dynamic_pointer_cast<UbranBlockGameInvariant>(invariants[key]);
        unsigned int component = t->getComponent(k);
        map<int, shared_ptr<GroupOfCells>> c = inv->getComponents();
        return c[component]; // If a cell isn't white, there exist a component that contains it.

    }
    else
        return {};
}

bool UrbanBlockGame::doTheComponentExist(const unsigned int &idnum, string const& color) {
    char key =  color == "#f00000" ? 'b' : color == "#00f000" ? 'g' : 0;
    if (key != 0){
        shared_ptr<UbranBlockGameInvariant> inv = dynamic_pointer_cast<UbranBlockGameInvariant>(invariants[key]);
        map<int, shared_ptr<GroupOfCells>> c = inv->getComponents();
        return c.find(idnum) != c.end();
    }
    else
        return false;
}

const map<char, std::shared_ptr<Invariant>> &UrbanBlockGame::getInvariants() const {
    return invariants;
}

vector<int> UrbanBlockGame::componentsIds(const string& color) {
    vector<int> ids;
    char key = color == "#f00000" ? 'b' : 'g';
    shared_ptr<UbranBlockGameInvariant> inv = dynamic_pointer_cast<UbranBlockGameInvariant>(invariants[key]);
    map<int, shared_ptr<GroupOfCells>> c = inv->getComponents();
    ids.reserve(c.size()); // Initialize the size of the vector
    for(const auto& x : c)
        ids.push_back(x.first);

    return ids;
}

std::shared_ptr<GroupOfCells> UrbanBlockGame::componentFromId(int idnum, const std::string& color) {
    char key = color == "#f00000" ? 'b' : 'g';
    shared_ptr<UbranBlockGameInvariant> inv = dynamic_pointer_cast<UbranBlockGameInvariant>(invariants[key]);
    map<int, shared_ptr<GroupOfCells>> c  = inv->getComponents();
    return c[idnum];
}
