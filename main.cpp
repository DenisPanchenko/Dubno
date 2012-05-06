#include <QApplication>
#include "MainWindow.h"
#include "FilterFactory.h"
#include "Configparser.h"

int main(int argc, char** argv){
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.setTitles("eng");
    mainWindow.show();
    return app.exec();
}
