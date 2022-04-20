#ifndef CUBE_CHANGECOLORSTRAT_H
#define CUBE_CHANGECOLORSTRAT_H

#include <string>
#include "Strategy.h"


class ChangeColorStrat: public  Strategy{
public:
    ChangeColorStrat(unsigned int k, std::string newColor);

    /**
     *
     * @param g is supposed to point to a UrbanBlockGame
     */
    void select(std::shared_ptr<Game> g) override;

    void unselect(std::shared_ptr<Game> g) override;

protected:
    void print(std::ostream &os) const override;

    [[nodiscard]] bool equal(const Strategy &other) const override;

public:

    ChangeColorStrat reverse();

    [[nodiscard]] unsigned int getK() const;

    [[nodiscard]] const std::string &getNewColor() const;

    [[nodiscard]] const std::string &getOldColor() const;

    [[nodiscard]] unsigned int getExpanded() const;

    [[nodiscard]] unsigned int getOldExpanded() const;

    [[nodiscard]] unsigned int getNewComponent() const;

    [[nodiscard]] unsigned int getOldComponent() const;

private:
    unsigned int k;
    std::string newColor;
    std::string oldColor;
    unsigned int expanded{};
    unsigned int oldExpanded{};
    unsigned int newComponent{};
    unsigned int oldComponent{};

};


#endif //CUBE_CHANGECOLORSTRAT_H
