#define FIXED_POINT 16

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "src/FileReader.h"
#include "src/FeedbackID.h"
#include "kiss_fft130/kiss_fft.h"
#include "kiss_fft130/tools/kiss_fftr.h"
#include "src/FFTAnalyzer.h"
#include <typeinfo>
#include "analyzerwindow.h"
#include <QApplication>
#include "chartview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AnalyzerWindow w;
    w.show();

    return a.exec();
}

/*
#define FIXED_POINT 16

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "FileReader.h"
#include "FeedbackID.h"
#include "kiss_fft.h"
#include "tools/kiss_fftr.h"
#include "../src/FFTAnalyzer.h"
#include <typeinfo>

using namespace std;

int main(int argc, char *argv[])
{
  FileReader reader;
  vector<int> data = reader.read("../wavfiles/example2.wav");

  FFTAnalyzer FFTtest(1024, 5, reader.getsamplerate());
  vector<vector<int> > analysis = FFTtest.fileAnalyze(data);

  FeedbackID IDtest(analysis);
  vector<vector<int> > FBProbs = IDtest.findFeedback();


} //http://soundfile.sapp.org/doc/WaveFormat/
*/
