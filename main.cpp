#define FIXED_POINT 16

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FileReader.h"
#include "kiss_fft.h"
#include "tools/kiss_fftr.h"
#include "../src/FFTAnalyzer.h"

using namespace std;

int main(int argc, char *argv[])
{
  FileReader reader;
  vector<int> data = reader.read("../wavfiles/example2.wav");

	FFTAnalyzer test(1024, 5);
	vector<vector<int> > analysis = test.fileAnalyze(data);

} //http://soundfile.sapp.org/doc/WaveFormat/

