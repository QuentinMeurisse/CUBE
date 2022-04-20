#include <QApplication>

#include "FLPWindow.h"

using namespace std;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FLPWindow frame;
    frame.show();
    QApplication::exec();
    return 1;
}
