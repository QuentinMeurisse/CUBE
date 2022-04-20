#include <QApplication>

#include "gap_window.h"
using namespace std;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GapWindow frame;
    frame.show();
    QApplication::exec();
    return 1;
}
