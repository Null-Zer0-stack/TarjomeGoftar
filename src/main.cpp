#include "TranslatorApp.h"
#include <QtWidgets/QApplication>
#include <QtCore/qstring.h>
int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("TarjomeGoftar");
    return a.exec();
}



