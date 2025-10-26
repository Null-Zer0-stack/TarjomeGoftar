#include "TranslatorApp.h"
#include <QtWidgets/QApplication>
#include <QtCore/qstring.h>
int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    a.setApplicationName("TarjomeGoftar");
    a.setApplicationVersion("1.1");
    MainWindow w;
    w.show();
    w.setWindowTitle("TarjomeGoftar");
    return a.exec();
}




