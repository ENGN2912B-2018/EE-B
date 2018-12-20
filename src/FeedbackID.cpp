#include "FeedbackID.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <typeinfo>
#include <stdexcept>

using namespace std;

/**
   The constuctor initalizes the varables and takes in a 2D vector of FFT data.
**/
FeedbackID::FeedbackID(vector<vector<int> > analysis)
{
  // FFT Data outputted by the FFTAnalyzer class
  data = analysis;
  iWidth = analysis.size();
  iHeight = analysis[1].size()/2;

  // Probability is broken up between 0 and MaxProb, with MaxProb representing 1.

  MaxProb_ = 32767;

  // A frequency with amplitude below the SNL Low threshold has probability of 0.
  // A frequency with amplitude above the SNL High theshold has probability of 1.
  // A frequency with amplitude between the thresholds has probability between 0 and 1 as a function of how close the amplitude is to the high threshold.
  SNLThresholdH_ = 15000;
  SNLThresholdL_ = 50;

  // A frequency with amplitude that is growing below the Swell Low threshold rate have a probability of 0.
  // A frequency with amplitude that is growing above the Swell Low threshold rate have a probability of 1.
  // A frequency with amplitude between the thresholds has probability between 0 and 1 as a function of how close the rate of change is to the high threshold.
  SwellThresholdH_ = 5000;
  SwellThresholdL_ = 50;

  // The Spectrum threshold is used to check the frequncies around the growing frequncy to check if they are growing at the same rate. This should be set the SwellThresholdL_ for best results.
  SpecThreshold_ = SwellThresholdL_;

  // The amount of neighboring frequncies to check.
  SpecMaxWidth_ = 10;

  // The percentage of probabilty that is added each time a frequency continues to act like feedback. For example,when set to 25, it will take 4 samples for the probabilty to become 1.
  probPerSustain_ = 25;

  // The ratio of fundamental frequency amplitude to harmonic frequency amplitude. Used to check if the offending frequency has harmonics.
  HarmonicRatio_ = 1;

  // The amount of harmonics to check.
  HarmonicMax_ = 4;

  // Weights of each of the checks, from 1 to 100, where 100 reprensents 100%.
  SNLWeight_ = 100;
  SwellWeight_ = 100;
  HarmonicWeight_ = 100;
  SpecWeight_ = 100;
  SustainWeight_ = 100;
}

FeedbackID::~FeedbackID() {}

/**
   The findMin functon returns the minimum value in each sample. Used for debugging.
**/
int findMin(vector<int> data)
{
  return *std::min_element(data.begin(),data.end());
}

/**
   The findMax functon returns the minimum value in each sample. Used for debugging.
**/
int findMax(vector<int> data)
{
  return *std::max_element(data.begin(),data.end());
}

/**
   A List of possable errors to be handled in coordination with the GUI.
 **/
void FeedbackID::checkValues(){

  if(SNLThresholdH_ <= 0 || SNLThresholdH_ > MaxProb_)
    cout << "RANGE ERROR: High SNL Threshold must be between 0 and " << MaxProb_ << endl;
  if(SNLThresholdL_ <= 0 || SNLThresholdL_ > MaxProb_)
    cout << "RANGE ERROR: Low SNL Threshold must be between 0 and " << MaxProb_ << endl;
  if(SNLThresholdL_ >= SNLThresholdH_)
    cout << "RANGE ERROR: Low SNL Threshold cannot be higher than High SNL Threshold" << endl;

  if(SwellThresholdH_ <= 0 || SwellThresholdH_ > MaxProb_)
    cout << "RANGE ERROR: High Swell Threshold must be between 0 and " << MaxProb_ << endl;
  if(SwellThresholdL_ <= 0 || SwellThresholdL_ > MaxProb_)
    cout << "RANGE ERROR: Low SNL Threshold must be between 0 and " << MaxProb_ << endl;
  if(SwellThresholdL_ >= SwellThresholdH_)
    cout << "RANGE ERROR: Low Swell Threshold cannot be higher than High Swell Threshold" << endl;

  if(SpecMaxWidth_ <= 0)
    cout << "RANGE ERROR: The width of the spectrum check must be greator than 0" << endl;

  if(probPerSustain_ <= 0 || probPerSustain_ >= 100)
    cout << "RANGE ERROR: The Probabilty addition for sustained frequancy per sample must be bwtween greator than 0 and less than 100" << endl;

  if(HarmonicRatio_ <= 0 || HarmonicRatio_ > 1)
    cout << "RANGE ERROR: The Ratio between harmonics must be greator than 0 and less than or equal to 1" << endl;

  if(HarmonicMax_ <= 0)
    cout << "RANGE ERROR: The amount of harmonics to check must be greator than 0" << endl;

  if(SNLWeight_ <= 0 || SNLWeight_ > 100)
    cout << "RANGE ERROR: The weight of the SNL check must be greator than 0 and less than or equal to 100" << endl;
  if(SwellWeight_ <= 0 || SwellWeight_ > 100)
    cout << "RANGE ERROR: The weight of the Swell check must be greator than 0 and less than or equal to 100" << endl;
  if(HarmonicWeight_ <= 0 || HarmonicWeight_ > 100)
    cout << "RANGE ERROR: The weight of the Harmonic check must be greator than 0 and less than or equal to 100" << endl;
  if(SpecWeight_ <= 0 || SpecWeight_ > 100)
    cout << "RANGE ERROR: The weight of the Specturm Width check must be greator than 0 and less than or equal to 100" << endl;
  if(SustainWeight_ <= 0 || SustainWeight_ > 100)
    cout << "RANGE ERROR: The weight of the Sustain check must be greator than 0 and less than or equal to 100" << endl;

}

/**
   The findFeedback function will return a matrix of probability in fixed point notation, so 0 is to 0.0 as 32767 is to 1.0. Each fixed point corresponds to about 0.0000305 of probability. 

   This function ONLY use information causally. Future inputs can not impact the feedback probability of the present.

   As such, the function will iterate through each sample of data, referencing previous samples and probabilities if needed.

   The probabiltes for each of the five checks are saved in their own 2D vector. The final vector is calculated by the weighted average of all 5 checks.

   Finally, the function returns the final probability vector.
 **/
vector<vector<int> > FeedbackID::findFeedback()
{
  // Ensure that the input makes sence and is in range, and that the data is not empty.
  checkValues();
  if(!data.empty()){

    // Initalize the corresponding 2D vector column and fill it with 0s. 
    vector<int> temp (data[0].size());
    int initProb = 0;
    fill(temp.begin(),temp.end(),initProb);

    // For every sample i, add an initalized vector to each probabilty vector and preform all 5 checks on the data sample. Then avaerge the probabilties up. 
    for (int i = 0; i < data.size(); i++) {
      probs.push_back(temp);
      SNLProbs.push_back(temp);
      SwellProbs.push_back(temp);
      HarmonicProbs.push_back(temp);
      SpecWidthProbs.push_back(temp);
      SustainProbs.push_back(temp);

      SNRCheck(i);
      SwellCheck(i);
      HarmonicCheck(i);
      //SpecWidthCheck(i);
      SpecWidthCheck2(i);
      SustainCheck(i);	
      Average(i);
    }
  }
  return probs;
}


void FeedbackID::SNRCheck(int i)
{
  int spacing = MaxProb_ / (SNLThresholdH_ - SNLThresholdL_);
  for (unsigned j = 0; j < data[i].size(); j++){
    if(data[i][j] >= SNLThresholdL_){
      if(data[i][j] >= SNLThresholdH_)
	SNLProbs[i][j] = MaxProb_;
      else
	SNLProbs[i][j] = (data[i][j] - SNLThresholdL_) * spacing;
    }
    else
      SNLProbs[i][j] = 0;
  }
}

void FeedbackID::SwellCheck(int i)
{
  int spacing = MaxProb_ / (SwellThresholdH_ - SwellThresholdL_);
  for (unsigned j = 0; j < data[i].size(); j++){
    if(i != 0){
      if(data[i][j] - data[i-1][j] >= SwellThresholdL_){
	if(data[i][j] - data[i-1][j] >= SwellThresholdH_)
	  SwellProbs[i][j] = MaxProb_;
	else
	  SwellProbs[i][j] = ((data[i][j] - data[i-1][j]) - SwellThresholdL_) * spacing; 
      }
      else
	SwellProbs[i][j] = 0;
    }
  }
}

void FeedbackID::HarmonicCheck(int i)
{
  int harmonicStep = MaxProb_ / HarmonicMax_;
  for (unsigned j = 0; j < data[i].size(); j++){
    unsigned count = 0;
    if((SwellProbs[i][j] >= 4000 || SNLProbs[i][j] >= 4000)){
      bool loopFlag = true;
      unsigned index = j;
      while(loopFlag && index + j < data[i].size()){
	if(SwellProbs[i][index+j] >= SwellProbs[i][index]){
	  count++;
	  index = index+j;
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

void FeedbackID::SpecWidthCheck(int i)
{
  int specStep = MaxProb_ / SpecMaxWidth_;
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
	if(j < data[i].size() - width)
	  if(data[i][j+width] - data[i-1][j+width] >= SpecThreshold_)
	    count++;
	  else
	    loopFlag = false;
	width++;
      }
      if(count >= SpecMaxWidth_)
	SpecWidthProbs[i][j] = MaxProb_;
      else
	SpecWidthProbs[i][j] = specStep * count;
    }
    else
      SpecWidthProbs[i][j] = 0;
  }
}

void FeedbackID::SpecWidthCheck2(int i)
{
  int specStep = MaxProb_ / SpecMaxWidth_;
  for(unsigned j = 0; j < data[i].size(); j++){
    if((SwellProbs[i][j] >= 1) && i != 0){
      bool loopFlag = true;
      unsigned width = 1;
      unsigned count = 0;
      unsigned check = SpecMaxWidth_;
      while(check != 0 && loopFlag){
	if(j >= width){
	  if(data[i][j-width] - data[i-1][j-width] >= SpecThreshold_){
	    count++;
	  }
	}
	if(j < data[i].size() - width){
	  if(data[i][j+width] - data[i-1][j+width] >= SpecThreshold_){
	    count++;
	  }
	}
	check--;
	width++;
      }
      if(count >= SpecMaxWidth_)
	SpecWidthProbs[i][j] = MaxProb_;
      else
	SpecWidthProbs[i][j] = specStep * count;
    }
    else
      SpecWidthProbs[i][j] = 0;
  }
}

void FeedbackID::SustainCheck(int i)
{
  for(unsigned j = 0; j < data[i].size(); j++){
    if(i != 0){
      if((SwellProbs[i][j] >= 1 || SNLProbs[i][j] >= 1) && (SwellProbs[i-1][j] >= 1 || SNLProbs[i-1][j] >= 1)){
	if(SustainProbs[i-1][j] + (((probPerSustain_/100))*MaxProb_ >= MaxProb_)){
	  SustainProbs[i][j] = MaxProb_;
	  }
	else
	  SustainProbs[i][j] =  SustainProbs[i-1][j] + (probPerSustain_/100*MaxProb_);	
      }
      else
	SustainProbs[i][j] = 0;
    }
  }
}

void FeedbackID::Average(int i)
{
  for(unsigned j = 0; j < probs[i].size(); j++){
    probs[i][j] = (SNLProbs[i][j]*((SNLWeight_/100)) + SwellProbs[i][j]*((SwellWeight_/100)) + HarmonicProbs[i][j]*((HarmonicWeight_/100.0))+ SpecWidthProbs[i][j]*((SpecWeight_/100)) + SustainProbs[i][j]*((SustainWeight_/100))) / (5.0);
  }

  if(i == 148){
    for(unsigned j = 0; j < probs[i].size(); j++){
      //cout << "SNLCHECK j = " << j << " Data: " << data[i][j] << " Prob:  " << SNLProbs[i][j]*((int)(SNLWeight_/100)) << endl;
      //cout << "SWELLCHECK j = " << j << " Data i-1: " << data[i-1][j] << " Data i: " << data[i][j] << " Prob: " << SwellProbs[i][j] << endl;
      //cout << "SWCHECK j = " << j << " Data i-1: " << data[i-1][j] << " Data i: " << data[i][j] <<  " Prob:  " << SpecWidthProbs[i][j] << endl;
      //cout << "SUSTAINCHECK j = " << j << " Data i-1: " << data[i-1][j] << " Data i: " << data[i][j] <<  " Prob:  " << SustainProbs[i][j] << endl;
      if(j*2 <= data[i].size()){
	//cout << "HarmonicCheck j = " << j << " Data i,j: " << data[i][j] << " Data i,j*2: " << data[i][j*2] <<  " Prob:  " << HarmonicProbs[i][j] << endl;
      }
      //cout << "Probs j = " << j << " Data: " << data[i][j] << " Prob:  " << probs[i][j] << endl;
    }
  }  
}

void FeedbackID::setSNLWeight(int value){
  SNLWeight_ = value;
}

void FeedbackID::setSwellWeight(int value){
  SwellWeight_ = value;
}

void FeedbackID::setHarmonicWeight(int value){
  HarmonicWeight_ = value;
}

void FeedbackID::setSpecWeight(int value){
  SpecWeight_ = value;
}

void FeedbackID::setSustainWeight(int value){
  SustainWeight_ = value;
}
