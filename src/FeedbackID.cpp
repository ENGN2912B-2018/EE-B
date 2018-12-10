#include "FeedbackID.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

FeedbackID::FeedbackID()
{
    //ctor
}

FeedbackID::~FeedbackID()
{
    //dtor
}

vector<vector<int> > FeedbackID::findFeedback(vector<vector<int> > data)
{
//This function will return a matrix of probability in fixed point notation, so 0 is to 0.0 as 32767 is to 1.0. Each fixed point corresponds to about 0.0000305 of probability.
//
//This function must ONLY use information causally. Future inputs can not impact the feedback probability of the present. 
//As such, the function will iterate through each sample of data, referencing previous samples and probabilities if needed.

	vector<vector<int> > probs; //generate output matrix. Maybe fill it with some initial probability?
	vector<int> temp (1024);
	int initProb = 32767/2; //0.5
	fill(temp.begin(),temp.end(),initProb);
	probs.push_back(temp);

	for (int i = 0; i < data.size(); i++) { // cycle through data matrix over time
		FeedbackID::SNRCheck(i, &probs, data);
		//SwellCheck(i, probs, data);
		//HarmonicCheck(i, probs, data);
		//SpecWidthCheck(i, probs, data);
		//SustainCheck(i,probs,data);	
	}
	
	return probs;
}

void FeedbackID::SNRCheck(int i, vector<vector<int> >* probs, vector<vector<int> > data)
{
	cout << "Living it up inside the SNR check function " <<  i << endl;
}
