#ifndef CUBE_UBRANBLOCKGAMEINVARIANT_H
#define CUBE_UBRANBLOCKGAMEINVARIANT_H


#include "Invariant.h"
#include "../GroupOfCells.h"
#include "../UrbanBlockGame.h"

#include <string>
#include <map>
#include <memory>

class UbranBlockGameInvariant : public Invariant {


public:
    UbranBlockGameInvariant(const std::shared_ptr<UrbanBlockGame> &g, std::string color);


    void update(const std::shared_ptr<Strategy> &strat) override;

    void undo(const std::shared_ptr<Strategy> &strat) override;

    [[nodiscard]] double getArea() const;

    [[nodiscard]] double getPerimeter() const;

    [[nodiscard]] unsigned int getNumComponents() const;

    [[nodiscard]] const std::map<int, std::shared_ptr<GroupOfCells>> &getComponents() const;

    virtual std::shared_ptr<GroupOfCells> createGroupOfCells(std::shared_ptr<Tiling> const& tiling,
                                                             unsigned int idnum,
                                                             std::string const& c);

protected:
    std::string color;
    double area = 0., perimeter = 0.;
    unsigned int numComponents = 0;
    std::map<int, std::shared_ptr<GroupOfCells>> components;
};


#endif //CUBE_UBRANBLOCKGAMEINVARIANT_H
