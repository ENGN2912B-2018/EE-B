#ifndef FEEDBACKID_H
#define FEEDBACKID_H
#include <iostream>
#include <vector>

using namespace std;

class FeedbackID
{
    public:
        FeedbackID(vector<vector<int> > data);
        ~FeedbackID();
        vector<vector<int> > findFeedback();
	void SNRCheck(int i);
	void SwellCheck(int i);
	void HarmonicCheck(int i);
	void SpecWidthCheck(int i);
	void SpecWidthCheck2(int i);
	void SustainCheck(int i);
	void Average(int i);
  
    protected:

    private:
	vector<vector<int> > data;

	vector<vector<int> > probs;
	vector<vector<int> > SNLProbs;
	vector<vector<int> > SwellProbs;
	vector<vector<int> > HarmonicProbs;
	vector<vector<int> > SpecWidthProbs;
	vector<vector<int> > SustainProbs;

	int MaxProb_;

	double SNLThresholdH_;
	double SNLThresholdL_;
	double SwellThresholdH_;
	double SwellThresholdL_;
	double SpecThreshold_;
	double SpecMaxWidth_;
	double probPerWidth_;
	double SustainThreshold_;
	double probPerSustain_;
	double HarmonicRatio_;
	double HarmonicMax_;

	double SNLWeight_;
	double SwellWeight_;
	double SpecWeight_;
	double SustainWeight_;
};

#endif // FEEDBACKID_H
