#define FIXED_POINT 16

#include "analyzerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnalyzerWindow w;
    w.show();

    return a.exec();
}
//http://soundfile.sapp.org/doc/WaveFormat/

