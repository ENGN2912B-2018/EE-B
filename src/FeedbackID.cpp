#include "FeedbackID.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>

using namespace std;

FeedbackID::FeedbackID()
{
    //ctor
}

FeedbackID::~FeedbackID()
{
    //dtor
}

int findMin(vector<int> data)
{
	//neglect first number in vector as it's the DC component. Should always be 0
	return *std::min_element(data.begin()+1,data.end());
}

int findMax(vector<int> data)
{
	return *std::max_element(data.begin(),data.end());
}

vector<vector<int> > FeedbackID::findFeedback(vector<vector<int> > data)
{
//This function will return a matrix of probability in fixed point notation, so 0 is to 0.0 as 32767 is to 1.0. Each fixed point corresponds to about 0.0000305 of probability.
//
//This function must ONLY use information causally. Future inputs can not impact the feedback probability of the present. 
//As such, the function will iterate through each sample of data, referencing previous samples and probabilities if needed.

	vector<int> temp (1024);
	int initProb = 0;
	fill(temp.begin(),temp.end(),initProb);

	for (int i = 0; i < data.size(); i++) { // cycle through data matrix over time
	  probs.push_back(temp);
	  SNLProbs.push_back(temp);
	  SwellProbs.push_back(temp);
	  HarmonicProbs.push_back(temp);
	  SpecWidthProbs.push_back(temp);
	  SustainProbs.push_back(temp);

	  SNRCheck(i, data);
	  SwellCheck(i, data);
	  HarmonicCheck(i, data);
	  SpecWidthCheck(i, data);
	  SustainCheck(i, data);	
	  Average(i, data);
	}
	return probs;
}

// How should the probabilty change as the mag approchaces the transform? 
void FeedbackID::SNRCheck(int i, vector<vector<int> > data)
{
  //cout << "Living it up inside the SNR check function " << i << endl;
  //cout << "Max of " << i <<  " = " << findMax(data[i]) << endl;
  int spacing = (SNLThresholdH_ - SNLThresholdL_) / 32767;
  for (unsigned j = 0; j < data[i].size(); j++){
    if(data[i][j] >= SNLThresholdL_){
      if(data[i][j] >= SNLThresholdH_)
	SNLProbs[i][j] = 32767;
      else
	SNLProbs[i][j] = (data[i][j] - SNLThresholdL_) * spacing;
    }
    else
      SNLProbs[i][j] = 0;
  }
}

void FeedbackID::SwellCheck(int i, vector<vector<int> > data)
{
  int spacing = 32767 / (SwellThresholdH_ - SwellThresholdL_);
  for (unsigned j = 0; j < data[i].size(); j++){
    if(i != 0){
      if(data[i][j] - data[i-1][j] >= SwellThresholdL_){
	if(data[i][j] - data[i-1][j] >= SwellThresholdH_)
	  SwellProbs[i][j] = 32767;
	else
	  SwellProbs[i][j] = ((data[i][j] - data[i-1][j]) - SwellThresholdL_) * spacing; 
      }
      else
	SwellProbs[i][j] = 0;
    }
  }
}

void FeedbackID::HarmonicCheck(int i, vector<vector<int> > data)
{
  int harmonicStep = 32767 / HarmonicMax_;
  for (unsigned j = 0; j < data[i].size(); j++){
    unsigned count = 0;
    if((SwellProbs[i][j] >= 1 || SNLProbs[i][j] >= 1)){
      bool loopFlag = true;
      unsigned index = j;
      while(loopFlag && index*2 < 1024){
	if(data[i][index*2] >= data[i][index]*HarmonicRatio_){
	  count++;
	  index = index*2;
	}
	else{
	  loopFlag = false;
	}
      }
      if(count >= HarmonicMax_){
	HarmonicProbs[i][j] = 0;
      }
      else{
	HarmonicProbs[i][j] = (HarmonicMax_ - count) * harmonicStep;
      }
    }
    else{
      HarmonicProbs[i][j] = 0;
    }
  }
}

void FeedbackID::SpecWidthCheck(int i, vector<vector<int> > data)
{
  for(unsigned j = 0; j < data[i].size(); j++){
    if((SwellProbs[i][j] >= 1) && i != 0){
      bool loopFlag = true;
      unsigned width = 1;
      unsigned count = 0;
      while(loopFlag){
	if(j >= width)
	  if(data[i][j-width] - data[i-1][j-width] >= SpecThreshold_)
	    count++;
	  else
	    loopFlag = false;
	if(j <= 1024 - width)
	  if(data[i][j+width] - data[i-1][j+width] >= SpecThreshold_)
	    count++;
	  else
	    loopFlag = false;
	width++;
      }
      if(count >= 10)
	SpecWidthProbs[i][j] = 32767;
      else
	SpecWidthProbs[i][j] = 3276 * count;
    }
    else
      SpecWidthProbs[i][j] = 0;
  }
}

void FeedbackID::SustainCheck(int i, vector<vector<int> > data)
{
  for(unsigned j = 0; j < data[i].size(); j++){
    if(i != 0){
      if((SwellProbs[i][j] >= 1 || SNLProbs[i][j] >= 1) && (SwellProbs[i-1][j] >= 1 || SNLProbs[i-1][j] >= 1)){
	if(SustainProbs[i-1][j] + (probPerSustain_*32767) >= 32767){
	  SustainProbs[i][j] = 32767;
	}
	else
	  SustainProbs[i][j] =  SustainProbs[i-1][j] + (probPerSustain_*32767);	
      }
      else
	SustainProbs[i][j] = 0;
    }
  }
}

void FeedbackID::Average(int i, vector<vector<int> > data)
{
  for(unsigned j = 0; j < probs[i].size(); j++){
    probs[i][j] = (SNLProbs[i][j]*SNLWeight_ + SwellProbs[i][j]*SwellWeight_ + SpecWidthProbs[i][j]*SpecWeight_ + SustainProbs[i][j]*SustainWeight_) / 4;
  }

  if(i == 148){
    for(unsigned j = 0; j < probs[i].size(); j++){
      //cout << "SNLCHECK j = " << j << " Data: " << data[i][j] << " Prob:  " << SNLProbs[i][j] << endl;
      //cout << "SWELLCHECK j = " << j << " Data i-1: " << data[i-1][j] << " Data i: " << data[i][j] << " Prob: " << SwellProbs[i][j] << endl;
      //cout << "SWCHECK j = " << j << " Data i-1: " << data[i-1][j] << " Data i: " << data[i][j] <<  " Prob:  " << SpecWidthProbs[i][j] << endl;
      //cout << "SUSTAINCHECK j = " << j << " Data i-1: " << data[i-1][j] << " Data i: " << data[i][j] <<  " Prob:  " << SustainProbs[i][j] << endl;
      if(j*2 <= 1024){
	//cout << "HarmonicCheck j = " << j << " Data i,j: " << data[i][j] << " Data i,j*2: " << data[i][j*2] <<  " Prob:  " << HarmonicProbs[i][j] << endl;
      }
    }
  }

  
}
