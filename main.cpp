#define FIXED_POINT 16

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "FileReader.h"
#include "kiss_fft.h"
#include "tools/kiss_fftr.h"
#include "../src/FFTAnalyzer.h"

using namespace std;

int main(int argc, char *argv[])
{
	FileReader reader;
	vector<int> data = reader.read("../wavfiles/example2.wav");

	FFTAnalyzer test(1024, 5, reader.getsamplerate());
	vector<vector<int> > analysis = test.fileAnalyze(data);

for (int i = 0; i < analysis.size(); i++) {
	cout << "Analyzing chunk " << i+1 << " for max and min. ";
	vector<int> temp = analysis[i];
	int max = 0; int min = 32000;
	int maxindex; int minindex;
	for (int k = 0; k < temp.size(); k++) { //max/min
		if (temp[k] > max) {max = temp[k]; maxindex = k;}
		if ((i > 5) && (temp[k] < min)) {min = temp[k]; minindex = k;}
	}
	cout << "Max = " << max << "@" << maxindex << "; Min = " << min << "@" << minindex << endl;
}

} //http://soundfile.sapp.org/doc/WaveFormat/

