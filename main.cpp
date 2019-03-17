#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QTextCodec * code = QTextCodec::codecForName("UTF-8");
    //QTextCodec * code = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(code);
    QTextCodec::setCodecForLocale(code);
    QTextCodec::setCodecForCStrings(code);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
