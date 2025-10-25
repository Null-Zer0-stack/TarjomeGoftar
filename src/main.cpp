#include "TranslatorApp.h"
#include <QtWidgets/QApplication>
#include <QtCore/qstring.h>
int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
   // QString app_name = "TarjomeGoftar";
   // a.applicationName = app_name;
    MainWindow w;
    w.show();
    w.setWindowTitle("TarjomeGoftar");
    return a.exec();
}


