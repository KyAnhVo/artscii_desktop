#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "artscii_processor.h"
#include "img_manager.h"
#include "main_window.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Main_Window win;
    win.resize(1024, 768);
    win.setVisible(true);

    return app.exec();
}
