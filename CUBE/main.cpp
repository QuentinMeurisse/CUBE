#include <QApplication>

#include "CUBELauncher.h"


using namespace std;
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CUBELauncher frame;
    frame.show();
    QApplication::exec();
    return 1;
}
