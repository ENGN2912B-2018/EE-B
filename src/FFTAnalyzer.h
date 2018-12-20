#ifndef FFTANALYZER_H
#define FFTANALYZER_H
#define PI 3.14159265358979
#define FIXED_POINT 16
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <kiss_fft130/kiss_fft.h>
#include <kiss_fft130/_kiss_fft_guts.h>
#include <kiss_fft130/tools/kiss_fftr.h>

using namespace std;

class FFTAnalyzer
{
    public:

FFTAnalyzer(unsigned vS, unsigned aP, unsigned sR): vecSize(vS), analysisPeriod(aP), sampleRate(sR) {
	cout << "Beginning " << vecSize << "-pt analysis of " << sampleRate << " Hz signal" << endl;
	}

~FFTAnalyzer() {}

//vars
    unsigned inputLength;
    unsigned vecSize;
    unsigned buffCount;
    unsigned buffLeftover;
    unsigned analysisPeriod;
    unsigned sampleRate;
    vector<vector<int> > signaldata;

std::vector<std::vector<int> > fileAnalyze(std::vector<int> data) {
	//std::cout << "Inside analyzer function." << std::endl;
//define some vars
	inputLength = data.size();
	buffCount = inputLength/vecSize;
	buffLeftover = inputLength%vecSize;
	bool isLeftovers;
	vector<vector<int> > matrixoutput;
	kiss_fft_scalar inputfft[vecSize];
    vector<int> debuginput;
    for (int i = 0; i < vecSize; i++) {debuginput.push_back(0);}
	kiss_fft_cpx outputfft[vecSize];
	kiss_fftr_cfg cfg;
    cfg = kiss_fftr_alloc(vecSize,0,NULL,NULL);



////analyze chunks periodically
    if (buffLeftover > 0) {buffCount = buffCount + 1; isLeftovers = true;} // if leftovers exist, increase buffer count by 1
    for (int i = 0; i < buffCount; i++) {



        if (i%analysisPeriod == 0) {

            if ((i+1 == buffCount) && (isLeftovers == true)) { // pad with zeros if at last chunk
                //if final chunk & leftovers exist, fill with zeros
                for (int k = 0; k < vecSize; k++) { // fill with zeros
                    inputfft[k+1] = 0;
                    debuginput[k+1] = 0;
                }
                //push leftovers into vector from the front
                for (int k = 0; k < buffLeftover; k++) { // fill beginning with leftovers
                    inputfft[k+1] = data[(i*vecSize)+1];
                    debuginput[k+1] = data[(i*vecSize)+1];
                }

            } else {
                //fill input buffer normally
                for (int k = 0; k < vecSize; k++) {
                    unsigned long l = (i*vecSize)+1+k;
                    //cout << "Filling " << k+1 << "/" << vecSize << " with data[" << l << "] = " << data[l] << endl;
                    inputfft[k] = data[l];
                    debuginput[k] = data[l];
                }

            }

//    for (int k = 0; k < vecSize; k++) {
//        double intcos = cos((PI/2)*k);
//        inputfft[k] = (int)(intcos);
//        debuginput.push_back((int)(intcos));
//    }
			//cout << "about to call fftr... " << endl;
//TODO: Apply window to input fft here!
//            for (int i = 0; i < vecSize; i++) {
//                double multiplier = 0.5 * (1 - cos(2*PI*i/(vecSize-1)));
//                inputfft[i] = multiplier * inputfft[i];
//            }
            //open ofstream to push signal data into fftin
            ofstream fftwI;
            fftwI.open("fftin");
            for (int k = 0; k < vecSize-1; k++) {
                fftwI << inputfft[k] << endl;
            }
            fftwI << inputfft[vecSize];
            fftwI.close();

            signaldata.push_back(debuginput);

//TODO: How to call fftwIO?

			kiss_fftr(cfg, inputfft, outputfft);
			vector<int> temp;

//pull fftout back in to temp vector. Keep an eye on max value

            string line;
            double normalizer = 13;
            double mag, max;
            max = 0;
            ifstream fft("fftout");
            if (fft.is_open())
            {
                while(getline(fft,line))
                {
                    mag = atof(line.c_str());
                    if (mag > max) {max = mag;}
                    temp.push_back((int)(mag/normalizer));
                }
            }
            //fill rest of vector with zeros
            for (int k = (vecSize/2+1); k < vecSize; k++) {
                temp.push_back(0);
            }

//			for (int k = 0; k < vecSize; k++) {
//				//cout << "find magnitude of " << outputfft[k+1].r << " + i" << outputfft[k+1].i << endl;
//                double absmag = sqrt(pow(outputfft[k+1].r,2) + pow(outputfft[k+1].i,2));
//                double dbmag = 10*log(absmag/131148); // relative is max fft output value for int input
//                double noisefloor = 60;
//                if(dbmag < -noisefloor) {dbmag = -noisefloor;}
//                if(dbmag > 0) {dbmag = 0;}
//                dbmag = (dbmag+noisefloor*1.00001)*(32000/noisefloor); // translate [-nf,0] to [0,nf] and then [0,32787]
//                temp.push_back((int)(dbmag));
//			}

			matrixoutput.push_back(temp);
        }
    }
float time = (float)(inputLength)/(float)(sampleRate);
float period = (float)(vecSize)*(float)(analysisPeriod)/(float)(sampleRate);
std::cout << fixed << setprecision(3);
std::cout << "Analyzed " << time << "s of audio every " << period << "s" << endl;
std::cout << "Returning output matrix of dimensions " << matrixoutput.size() << "x" << matrixoutput[0].size() << "." << endl;
	return matrixoutput;
}

    protected:

    private:
};

#endif // FFTANALYZER_H
