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
  vector<int> data = reader.read("example2.wav");

  for (vector<int>::iterator i = data.begin(); i != data.end(); ++i){
    //cout << *i << endl;
  }

  cout << "Initializing vector for analysis" << endl;
  unsigned vecSize = 5000;
  kiss_fft_scalar input[vecSize];
  cout << "input vector of size " << vecSize << ": ";

  for (int i = 0; i < vecSize; i++) {
    input[i] = data[i];
  }
		
  cout << "\nSuccessfully generated vector of length " << vecSize << endl;
  cout << "Prepping kiss_fft for FFT" << endl;

  kiss_fftr_cfg cfg;
  cfg = kiss_fftr_alloc(vecSize, 0,0,0);
  cout << "Pushed input vector into cx_in" << endl;
  cout << "Initializing output vector. Calling fftr" << endl;
  kiss_fft_cpx output[vecSize];
  cout << "HI" << endl;
  kiss_fftr(cfg, input, output);
  cout << "Made it past fftr" << endl;
	
  for (int i = 0; i < vecSize; i++) {
    cout << "Mag = " <<sqrt(pow(output[i].r,2) + pow(output[i].i,2)) << " : ";
    cout << output[i].r << " + " << output[i].i << "i";
    cout << endl;
  }

  
}

//http://soundfile.sapp.org/doc/WaveFormat/



