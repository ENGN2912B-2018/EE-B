#ifndef FFTANALYZER_H
#define FFTANALYZER_H
#include <iostream>
#include <vector>

class FFTAnalyzer
{
    public:
		FFTAnalyzer(unsigned vS, unsigned aP): vecSize(vS), analysisPeriod(aP) {
			std::cout << "Class initialized with vecSize " << vecSize << " and analysisPeriod " << analysisPeriod << std::endl;
		}
        ~FFTAnalyzer() {}
        std::vector<std::vector<int> > fileAnalyze(std::vector<int> data) {}

    protected:

    private:
		unsigned inputLength;
		unsigned vecSize;
		unsigned buffCount;
		unsigned buffLeftover;
		unsigned analysisPeriod;
};

#endif // FFTANALYZER_H
