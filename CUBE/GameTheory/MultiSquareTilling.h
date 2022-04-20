#ifndef CUBE_MULTISQUARETILLING_H
#define CUBE_MULTISQUARETILLING_H

#include "RegularTiling.h"

class MultiSquareTilling : public RegularTiling {
public:
    MultiSquareTilling(int sub_width, int sub_height, const double &cellsArea, int blocksWidth,
                       int blocksHeight, int blocksDistance);

    [[nodiscard]] int getWidth() const override;

    [[nodiscard]] int getHeight() const override;

protected:
    void generate() override;

private:
    int blocks_width;
    int blocks_height;
    int blocks_distance;

    void generate(double startx, double straty);
};


#endif //CUBE_MULTISQUARETILLING_H
