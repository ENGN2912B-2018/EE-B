#include "FFTAnalyzer.h"
#include <iostream>
#include <vector>


using namespace std;

FFTAnalyzer::fileAnalyze(vector<int> data) {

	cout << "Beginning fft analysis..." << endl;
	inputLength = data.size();
	cout << "test1";
	buffCount = inputLength/vecSize;
	cout << "test2";
	buffLeftover = inputLength%vecSize;
	cout << inputLength << " " << buffCount << " " << buffLeftover << endl;
	
	//initialize output analysis matrix

	vector<vector<int> > outputMatrix(buffCount+1);
	for (int i = 0; i < vecSize; i++) {
		outputMatrix.resize(vecSize);
	}
	
  
/*
	cout << "Initializing vector for analysis" << endl;
	kiss_fft_scalar inputfft[vecSize];
	cout << "input vector of size " << vecSize << ": ";

	for (int i = 0; i < vecSize; i++) {	
		inputfft[i] = data[i];
	}
		
	cout << "\nSuccessfully generated vector of length " << vecSize << endl;
	cout << "Prepping kiss_fft for FFT" << endl;

	kiss_fftr_cfg cfg;
	cfg = kiss_fftr_alloc(vecSize, 0,0,0);
	cout << "Pushed input vector into cx_in" << endl;
	cout << "Initializing output vector. Calling fftr" << endl;
	kiss_fft_cpx outputfft[vecSize];
	cout << "HI" << endl;
	kiss_fftr(cfg, inputfft, outputfft);
	cout << "Made it past fftr" << endl;
	
	//for (int i = 0; i < vecSize; i++) {
	//  cout << "Mag = " <<sqrt(pow(outputfft[i].r,2) + pow(outputfft[i].i,2)) << " : ";
	//  cout << outputfft[i].r << " + " << outputfft[i].i << "i";
	//  cout << endl;
	//}
*/

	return outputMatrix;

};
