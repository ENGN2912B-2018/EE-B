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
#include "gnuplot_i.hpp"

using namespace std;
void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}
int main(int argc, char *argv[])
{
  // The FileReader class reads in the header and data from a .wav file.
  FileReader reader;
  vector<int> data = reader.read("../wavfiles/example2.wav");

  // The FFTAnalyzer takes the FFT of the .wav file for data. The fileAnalyze method outputs the data in 2D vector format.
  FFTAnalyzer FFTtest(1024, 5, reader.getsamplerate());
  vector<vector<int> > analysis = FFTtest.fileAnalyze(data);

  // The FeedbackID class calculates the probability of feedback occuring at each frequancy for each sample.
  FeedbackID IDtest(analysis);
  vector<vector<int> > FBProbs = IDtest.findFeedback();
      
  vector<vector<double> > probs = IDtest.toDouble();

  // Gnuplot gp;
  // gp << "set pm3d map \n";
  // gp << "set yrange[0:175] \n";
  // gp << "set xrange[1:1023] \n";
  // gp << "set zrange[0:1] \n";
  // gp << "splot '-'\n";
  // gp.send2d(probs);

  Gnuplot g9;
  cout << "window 9: plot_image" << endl;
  const int iWidth  = probs[0].size();
  const int iHeight = probs.size();
  g9.set_xrange(0,iWidth).set_yrange(0,iHeight).set_cbrange(0,100);
  //g9.cmd("set palette gray");
  unsigned char ucPicBuf[iWidth*iHeight];
  // generate a greyscale image
  for(int iIndex = 0; iIndex < iHeight; iIndex++){
    for(int jIndex = 0; jIndex < iWidth; jIndex++){
      ucPicBuf[iWidth*iIndex+jIndex] = (int)100*probs[iIndex][jIndex];
    }
  }
  g9.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
  
  wait_for_key();

} // http://soundfile.sapp.org/doc/WaveFormat/

