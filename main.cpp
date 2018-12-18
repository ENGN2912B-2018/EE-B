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
#include "gnuplot-iostream.h"

using namespace std;

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

  Gnuplot gp;
  gp << "set pm3d map \n";
  gp << "set yrange[0:175] \n";
  gp << "set xrange[1:1023] \n";
  gp << "set zrange[0:1] \n";
  gp << "splot '-'\n";
  gp.send2d(probs);
        

} // http://soundfile.sapp.org/doc/WaveFormat/

