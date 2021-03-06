#include <iostream>
#include <vector>
#include <string>
#include "kiss_fft.h"
#include "tools/kiss_fftr.h"
#define FIXED_POINT 16

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {

	cout << "Initializing vector for analysis" << endl;
	unsigned vecSize = argc - 1;
	kiss_fft_scalar input[vecSize];
	cout << "input vector of size " << vecSize << ": ";
	for (int i = 0; i < vecSize; i++) {
		int x = atoi(argv[i+1]);
		cout << x << " ";
		input[i] = x;
	}
		
	cout << "\nSuccessfully generated vector of length " << vecSize << endl;
	cout << "Prepping kiss_fft for FFT" << endl;

	kiss_fftr_cfg cfg;
	cfg = kiss_fftr_alloc(vecSize, 0,0,0);
	//cout << "Pushed input vector into cx_in" << endl;
	//cout << "Initializing output vector. Calling fftr" << endl;
	kiss_fft_cpx output[vecSize];
	kiss_fftr(cfg, input, output);
	//cout << "Made it past fftr" << endl;
	
	for (int i = 0; i < vecSize; i++) {
		cout << "Mag = " <<sqrt(pow(output[i].r,2) + pow(output[i].i,2)) << " : ";
		cout << output[i].r << " + " << output[i].i << "i";
		cout << endl;
	}


}
