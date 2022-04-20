#include <QApplication>

#include "GAPWindow.h"
using namespace std;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GAPWindow frame;
    frame.show();
    QApplication::exec();
    return 1;
}
