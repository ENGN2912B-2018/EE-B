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

