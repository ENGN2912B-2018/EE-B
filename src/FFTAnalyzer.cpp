#include "FFTAnalyzer.h"
#include "kiss_fft.h"
#include "tools/kiss_fftr.h"
#include "_kiss_fft_guts.h"
#include <iostream>
#include <vector>
//#include <math>

using namespace std;

vector<vector<int> > FFTAnalyzer::fileAnalyze(vector<int> data) {
//define some vars
	inputLength = data.size();
	buffCount = inputLength/vecSize;
	buffLeftover = inputLength%vecSize;
	bool isLeftovers;
	vector<vector<int> > matrixoutput;
	kiss_fft_scalar inputfft[vecSize];
	kiss_fft_cpx outputfft[vecSize];
	kiss_fftr_cfg cfg;
	cfg = kiss_fftr_alloc(vecSize,0,0,0);
//analyze chunks periodically
	if (buffLeftover > 0) {buffCount = buffCount + 1; isLeftovers = true;} // if leftovers exist, increase buffer count by 1
	for (int i = 0; i < buffCount; i++) {
		if (i%analysisPeriod == 0) {
			if ((i < buffCount-1) && (isLeftovers == true)) { // pad with zeros if at last chunk
				for (int k = 0; k < vecSize; k++) { // fill with zeros
					inputfft[k+1] = 0;
				}
				for (int k = 0; k < buffLeftover; k++) { // fill beginning with leftovers
					inputfft[k+1] = data[(i*vecSize)+1];
				}
			} else {
				for (int k = 0; k < vecSize; k++) {
					inputfft[k+1] = data[(i*vecSize)+1];
				}
			}
			kiss_fftr(cfg, inputfft, outputfft);
			vector<int> temp;
			for (int k = 0; k < vecSize; k++) {
				temp[k+1] = sqrt(pow(outputfft[k+1].r,2) + pow(outputfft[k+1].i,2));
			}
			matrixoutput.push_back(temp);
		}
	}	

	
	return matrixoutput;

}
