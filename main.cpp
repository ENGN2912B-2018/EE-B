#define FIXED_POINT 16

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FileReader.h"
#include "kiss_fft.h"
#include "tools/kiss_fftr.h"

using namespace std;

int main(int argc, char *argv[])
{
  FileReader reader;
  vector<int> data = reader.read("../wavfiles/example2.wav");

  // initialize buffer vars
  unsigned inputLength = data.size();
  unsigned vecSize = 1024; // ideally should be power of 2
  unsigned buffCount = inputLength/vecSize;
  unsigned buffLeftover = inputLength % vecSize;
  unsigned analysisPeriod = 5; //This var determines how many buffers to wait before analyzing again
  cout << "buffer vars: " << inputLength << " " << vecSize << " " << buffCount << " " << buffLeftover << " " << analysisPeriod << endl;

  //initialize output analysis matrix
  
  for (unsigned i = 0; i < buffLeftover; ++i){
    data.push_back(0);
  }

  vector<vector<double>> ffts;
  for(unsigned i = 0; i <= buffCount; i++){
    if(i%analysisPeriod == 0){
      cout << "Initializing vector for analysis i = " << i << endl;
      cout << "about to create an array with kiss scalar type..." << endl;
      kiss_fft_scalar inputfft[vecSize];
      cout << "input vector of size " << vecSize << ": ";

      for (int j = 0; j < vecSize; j++) {
	inputfft[j] = data[j+i*vecSize];
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

      vector<double> mag;

      for (int i = 0; i < vecSize; i++) {
	mag.push_back(sqrt(pow(outputfft[i].r,2) + pow(outputfft[i].i,2)));
	cout << "Mag = " <<sqrt(pow(outputfft[i].r,2) + pow(outputfft[i].i,2)) << " : ";
	cout << outputfft[i].r << " + " << outputfft[i].i << "i";
	cout << endl;
      }
      ffts.push_back(mag);
    } 
  }  
}

//http://soundfile.sapp.org/doc/WaveFormat/



