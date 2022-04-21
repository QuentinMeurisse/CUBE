#include <QApplication>

#include "GUI/GameFrame.h"


using namespace std;
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    GameFrame frame;
    frame.show();
    QApplication::exec();
    return 1;
}
