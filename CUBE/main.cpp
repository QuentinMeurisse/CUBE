#include <iostream>
#include <QApplication>

#include "GUI/CoModFrame.h"

using namespace std;
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CoModFrame frame;
    frame.show();
    QApplication::exec();
    return 1;
}
