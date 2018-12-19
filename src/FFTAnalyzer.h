#ifndef FFTANALYZER_H
#define FFTANALYZER_H
#define PI 3.14159265358979
#define FIXED_POINT 16
#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;

class FFTAnalyzer
{
    public:

FFTAnalyzer(unsigned vS, unsigned aP, unsigned sR): vecSize(vS), analysisPeriod(aP), sampleRate(sR) {
	cout << "Beginning " << vecSize << "-pt analysis of " << sampleRate << " Hz signal" << endl;
	}

~FFTAnalyzer() {}

std::vector<std::vector<int> > fileAnalyze(std::vector<int> data){
	//std::cout << "Inside analyzer function." << std::endl;
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
	//cout << "Vars defined/allocated" << endl;
//analyze chunks periodically
	if (buffLeftover > 0) {buffCount = buffCount + 1; isLeftovers = true;} // if leftovers exist, increase buffer count by 1
	for (int i = 0; i < buffCount; i++) {
		//cout << "Chunk = " << i+1 << "/" << buffCount;
		if (i%analysisPeriod == 0) {
			if ((i+1 == buffCount) && (isLeftovers == true)) { // pad with zeros if at last chunk
				//cout << "inside leftover if statement" << endl;
				for (int k = 0; k < vecSize; k++) { // fill with zeros
					inputfft[k+1] = 0;
				}
				for (int k = 0; k < buffLeftover; k++) { // fill beginning with leftovers
					inputfft[k+1] = data[(i*vecSize)+1];
				}
			} else {
				//cout << "Inside normal if statement" << endl;
				for (int k = 0; k < vecSize; k++) {
					int l = (i*vecSize)+1+k;
					//cout << "Filling " << k+1 << "/" << vecSize << " with data[" << l << "] = " << data[l] << endl;
					inputfft[k] = data[l];
				}
			}
			//cout << "about to call fftr... " << endl;
//TODO: Apply window to input fft here!
//            for (int i = 0; i < vecSize; i++) {
//                double multiplier = 0.5 * (1 - cos(2*PI*i/(vecSize-1)));
//                inputfft[i] = multiplier * inputfft[i];
//            }
			kiss_fftr(cfg, inputfft, outputfft);
			//cout << "initializing output vector to be pushed into matrix" << endl;
			vector<int> temp;
			//cout << "MADE IT HERE!!" << endl;
			for (int k = 0; k < vecSize; k++) {
				//cout << "find magnitude of " << outputfft[k+1].r << " + i" << outputfft[k+1].i << endl;
                double absmag = sqrt(pow(outputfft[k+1].r,2) + pow(outputfft[k+1].i,2));
                double dbmag = 10*log(absmag/131148); // relative is max fft output value for int input
                double noisefloor = 60;
                if(dbmag < -noisefloor) {dbmag = -noisefloor;}
                if(dbmag > 0) {dbmag = 0;}
                dbmag = (dbmag+noisefloor*1.00001)*(32000/noisefloor); // translate [-nf,0] to [0,nf] and then [0,32787]
                temp.push_back((int)(dbmag));
			}
            //Smooth out the frequencies by averaging
//            for (int i = 0; i < temp.size(); i++) {
//                if (i > 2 && i < vecSize-3) {
//                    temp[i] = (temp[i-2]+temp[i-1]+temp[i]+temp[i+1]+temp[i+2])/5;
//                }
//            }
			//cout << "Filled vector with magnitude of fftr. Pushing to output matrix... " << endl;
			matrixoutput.push_back(temp);
			//cout << "pushed back! i = " << i << endl;
		}
	}
float time = (float)(inputLength)/(float)(sampleRate);
float period = (float)(vecSize)*(float)(analysisPeriod)/(float)(sampleRate);
cout << fixed << setprecision(3);	
cout << "Analyzed " << time << "s of audio every " << period << "s" << endl;
cout << "Returning output matrix of dimensions " << matrixoutput.size() << "x" << matrixoutput[0].size() << "." << endl;
	return matrixoutput;

}

	//vars	
		unsigned inputLength;
		unsigned vecSize;
		unsigned buffCount;
		unsigned buffLeftover;
		unsigned analysisPeriod;
		unsigned sampleRate;

    protected:

    private:
};

#endif // FFTANALYZER_H
