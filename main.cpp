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

  for (vector<int>::iterator i = data.begin(); i != data.end(); ++i){
    //cout << *i << endl;
  } // what is the purpose of this?

  // initialize buffer vars
  unsigned inputLength = data.size();
  unsigned vecSize = 1024; // ideally should be power of 2
  unsigned buffCount = inputLength/vecSize;
  unsigned buffLeftover = inputLength % vecSize;
  unsigned analysisPeriod = 5; //This var determines how many buffers to wait before analyzing again
  cout << "buffer vars: " << inputLength << " " << vecSize << " " << buffCount << " " << buffLeftover << " " << analysisPeriod << endl;

  //initialize output analysis matrix
  

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

  
}

//http://soundfile.sapp.org/doc/WaveFormat/



