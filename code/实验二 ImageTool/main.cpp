#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int id = QFontDatabase::addApplicationFont("/usr/share/fonts/FangSong.ttf");
    if(id<0)
    {
    qDebug("Cannot open the font file!");
    }else
    {
    QString mfont = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(mfont);
    a.setFont(font);
    }
    return a.exec();
}
