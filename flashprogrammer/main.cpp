#include <QApplication>
#include "mainwindow.h"
#include "qtextcodec.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    w.show();
    
    return a.exec();
}
