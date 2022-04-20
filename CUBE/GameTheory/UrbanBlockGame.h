#ifndef CUBE_URBANBLOCKGAME_H
#define CUBE_URBANBLOCKGAME_H

#include <memory>
#include <map>
#include <vector>
#include <optional>

#include "../GameCore/Game.h"
#include "../GameCore/Invariant.h"
#include "../GameCore/Strategy.h"
#include "Tiling.h"
#include "GroupOfCells.h"

class UrbanBlockGame : public Game{
public:
    explicit UrbanBlockGame(std::shared_ptr<Tiling> t);

    void addInvariant(std::shared_ptr<Invariant> inv, char const& key);

    [[nodiscard]] const std::shared_ptr<Tiling> &getT() const;

    void updateAll(const std::shared_ptr<Strategy> &s);

    void undoAll(const std::shared_ptr<Strategy> &s);

    /**
     *
     * @param k index of a cell
     * @return the GroupOfCell that contains the cell k if it exists
     */
    std::optional<std::shared_ptr<GroupOfCells>> getGroupOfCells(const unsigned int &k);

    bool doTheComponentExist(const unsigned int &idnum, const std::string& color);

    [[nodiscard]] const std::map<char, std::shared_ptr<Invariant>> &getInvariants() const;

    /**
     *
     * @param color supposed to be "#f00000" for buildings and "#00f000" for green spaces
     * @return
     */
    std::vector<int> componentsIds(const std::string& color);

    /**
     *
     * @param idnum supposed to be an existing index
     * @param color supposed to be "#f00000" or "#00f000"
     * @return
     */
    std::shared_ptr<GroupOfCells> componentFromId(int idnum, const std::string& color);

private:
    std::shared_ptr<Tiling> t;
    std::map<char, std::shared_ptr<Invariant>> invariants;
};


#endif //CUBE_URBANBLOCKGAME_H
