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
  // The FileReader class reads in the header and data from a .wav file.
  FileReader reader;
  vector<int> data = reader.read("../wavfiles/example2.wav");

  // The FFTAnalyzer takes the FFT of the .wav file for data. The fileAnalyze method outputs the data in 2D vector format.
  FFTAnalyzer FFTtest(1024, 5, reader.getsamplerate());
  vector<vector<int> > analysis = FFTtest.fileAnalyze(data);

  // The FeedbackID class calculates the probability of feedback occuring at each frequancy for each sample.
  FeedbackID IDtest(analysis);
  vector<vector<int> > FBProbs = IDtest.findFeedback();
        

} // http://soundfile.sapp.org/doc/WaveFormat/

