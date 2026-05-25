#include <QTimer>
#include "NisApplication.h"

int main(int argc, char* argv[]) {
    NisApplication app(argc, argv);
    QTimer::singleShot(0, &app, &NisApplication::run);
    return app.exec();
}
