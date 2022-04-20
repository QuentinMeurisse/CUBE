#include <QApplication>

#include "GUI/CoModFrame.h"
#include "GraphGame/GUI/GraphControlWindow.h"

using namespace std;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GraphControlWindow frame;
    frame.show();
    QApplication::exec();
    return 1;
}
