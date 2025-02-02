#include <QApplication>
#include "MainWindow/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Application-wide dark style.
    app.setStyleSheet(
        "QWidget { background-color: #2d2d30; color: #e0e0e0; }"
        "QMenuBar { background-color: #2d2d30; color: #e0e0e0; }"
        "QStatusBar { background-color: #2d2d30; color: #e0e0e0; }"
    );
    QApplication::setStyle("Fusion");

    MainWindow window;
    window.resize(900, 700);
    window.show();

    return app.exec();
}
