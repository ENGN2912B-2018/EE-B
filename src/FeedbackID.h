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
		void SNRCheck(int i, vector<vector<int> > *probs, vector<vector<int> > data);	
    protected:

    private:
		

};

#endif // FEEDBACKID_H
