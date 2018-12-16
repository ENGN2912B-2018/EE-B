#ifndef FEEDBACKID_H
#define FEEDBACKID_H
#include <iostream>
#include <vector>

using namespace std;

class FeedbackID
{
    public:
        FeedbackID();
        ~FeedbackID();
        vector<vector<int> > findFeedback(vector<vector<int> > data);
	void SNRCheck(int i, vector<vector<int> > data);
	void SwellCheck(int i, vector<vector<int> > data);
	void HarmonicCheck(int i, vector<vector<int> > data);
	void SpecWidthCheck(int i, vector<vector<int> > data);
	void SustainCheck(int i, vector<vector<int> > data);
	void Average(int i, vector<vector<int> > data);
  
    protected:

    private:
	vector<vector<int> > probs; //generate output matrix. Maybe fill it with some initial probability?
	vector<vector<int> > SNLProbs;
	vector<vector<int> > SwellProbs;
	vector<vector<int> > HarmonicProbs;
	vector<vector<int> > SpecWidthProbs;
	vector<vector<int> > SustainProbs;

	double SNLThresholdH_ = 800;
	double SNLThresholdL_ = 200;
	double SwellThresholdH_ = 2000;
	double SwellThresholdL_ = 400;
	double SpecThreshold_ = 400;
	double probPerWidth_ = .1;
	double SustainThreshold_ = -200;
	double probPerSustain_ = 0.25;
	double HarmonicRatio_ = 1;
	double HarmonicMax_ = 4;

	double SNLWeight_ = 1;
	double SwellWeight_ = 1;
	double SpecWeight_ = 1;
	double SustainWeight_ = 1;
		

};

#endif // FEEDBACKID_H
