#include <iostream>
#include <QApplication>
#include <set>
#include <memory>

#include "SquareTiling.h"
#include "GUI/DisplayTilingFrame.h"
#include "segment.h"
#include "group_of_cells3D.h"
#include "UrbanBlockGame.h"
#include "Building3DInvariant.h"
#include "GreenSpaces3DInvariant.h"
#include "Players/PorosityPlayer.h"
#include "Players/NumBuildingPlayer.h"
#include "Listeners/PrintListener.h"
#include "IterativeBest.h"
#include "Players/BuildingsPerimeterPlayer.h"
#include "Players/NumGreenSpacesPlayer.h"
#include "Players/GreenSpaceRatePlayer.h"
#include "IterativeBetterRandom.h"
#include "Buildings3DPerimeterPlayer.h"
#include "Strategies/InflateStrat.h"
#include "Inflate3DStrat.h"
#include "NumBuilding3DPlayer.h"
#include "HeightPlayer.h"


using namespace std;
using Mesh = OpenMesh::PolyMesh_ArrayKernelT<>;

void run(int argc, char* argv[]){
    shared_ptr<SquareTiling> t = make_shared<SquareTiling>(100, 100, 1);
    t->init();

    shared_ptr<UrbanBlockGame> ubg = make_shared<UrbanBlockGame>(t);
    shared_ptr<Building3DInvariant> binv = make_shared<Building3DInvariant>(ubg, 3);
    ubg->addInvariant(binv, 'b');
    shared_ptr<GreenSpaces3DInvariant> ginv = make_shared<GreenSpaces3DInvariant>(ubg);
    ubg->addInvariant(ginv, 'g');

    unsigned int rb = 4;
    unsigned int rg = 1;
    double angle = 45;
    double height_projection = 1;
    //freopen("../../../output.txt", "w", stdout);
    //shared_ptr<PorosityPlayer> porosity = make_shared<PorosityPlayer>(rb, rg);
    shared_ptr<NumBuilding3DPlayer> nb = make_shared<NumBuilding3DPlayer>(5, 20, rb, rg, angle, height_projection);
    //nb->setSeed(3431551209);
    cout << "Nb seed: " << nb->getSeed() << endl;
    shared_ptr<Buildings3DPerimeterPlayer> perim = make_shared<Buildings3DPerimeterPlayer>(75, 500, rb, rg, angle, height_projection);
    //perim->setSeed(1543557032);
    cout << "perim seed: " << perim->getSeed() << endl;
    shared_ptr<HeightPlayer> hp = make_shared<HeightPlayer>(3, angle, height_projection);
    cout << "hp seed: " << hp->getSeed() << endl;
    shared_ptr<NumGreenSpacesPlayer> ng = make_shared<NumGreenSpacesPlayer>(5, 6, rb, rg);
    //ng->setSeed(4096478485);
    cout << "ng seed: " << ng->getSeed() << endl;
    shared_ptr<GreenSpaceRatePlayer> gr = make_shared<GreenSpaceRatePlayer>(0.2, rb, rg);
    //gr->setSeed(3947099744);
    cout << "gr seed: " << gr->getSeed() << endl;
    vector<shared_ptr<Player>> players{nb, perim, hp, ng, gr};
    shared_ptr<PrintListener> l = make_shared<PrintListener>();
    IterativeBetterRandom gameplay(players, ubg, 1000);
    gameplay.addListener(l);

    gameplay.play();

    int i = 0;
    for (auto const& [k, b] : binv->getComponents()){
        shared_ptr<GroupOfCells3D> b3d = dynamic_pointer_cast<GroupOfCells3D>(b);
        b3d->write("../../../3dblock.json", i == 0);
        i++;
    }

    for (auto const& [k, g] : ginv->getComponents()){
        shared_ptr<GroupOfCells3D> g3d = dynamic_pointer_cast<GroupOfCells3D>(g);
        g3d->write("../../../3dblock.json");
    }

    QApplication app(argc, argv);
    DisplayTilingFrame frame(t, 10);
    frame.show();
    QApplication::exec();



}

int main(int argc, char* argv[]){
    run(argc, argv);
    /*shared_ptr<SquareTiling> t = make_shared<SquareTiling>(100, 100, 1);
    t->init();

    shared_ptr<UrbanBlockGame> ubg = make_shared<UrbanBlockGame>(t);
    shared_ptr<Building3DInvariant> binv = make_shared<Building3DInvariant>(ubg, 5);
    ubg->addInvariant(binv, 'b');
    shared_ptr<GreenSpaces3DInvariant> ginv = make_shared<GreenSpaces3DInvariant>(ubg);
    ubg->addInvariant(ginv, 'g');

    unsigned int rb = 4;
    unsigned int rg = 1;
    double angle = 45;
    double proj = 1;

    ChangeColorStrat cs(3830, "#f00000");
    cs.select(ubg);
    Inflate3DStrat i3ds(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    cs = ChangeColorStrat(1947, "#00f000");
    cs.select(ubg);
    InflateStrat is(2, "#00f000", rb, rg);
    is.select(ubg);

    cs = ChangeColorStrat(3869, "#f00000");
    cs.select(ubg);
    i3ds = Inflate3DStrat(3, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    cs = ChangeColorStrat(3415, "#00f000");
    cs.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    cs = ChangeColorStrat(603, "#f00000");
    cs.select(ubg);
    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    cs = ChangeColorStrat(6428, "#00f000");
    cs.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    cs = ChangeColorStrat(7879, "#f00000");
    cs.select(ubg);
    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    cs = ChangeColorStrat(2708, "#f00000");
    cs.select(ubg);
    i3ds = Inflate3DStrat(3, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(8, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(3, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(8, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(3, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(1, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(3, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(5, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    is = InflateStrat(2, "#00f000", rb, rg);
    is.select(ubg);

    is = InflateStrat(6, "#00f000", rb, rg);
    is.select(ubg);

    i3ds = Inflate3DStrat(7, "#f00000",rb, rg, angle, proj);
    i3ds.select(ubg);

    cout << "\n\n\n" << endl;

    freopen("../../../output.txt", "w", stdout);
    cout << "Inflate 4" << endl;
    is = InflateStrat(4, "#00f000", rb, rg);
    is.select(ubg);

    *//*for (int i = 0; i < t->numCells(); ++i) {
        if (t->getComponent(i) == 4)
            cout << i << endl;
    }*//*



    QApplication app(argc, argv);
    DisplayTilingFrame frame(t, 10);
    frame.show();
    QApplication::exec();*/

    return 0;
}