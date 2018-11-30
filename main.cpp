#include <iostream>
#include <fstream>
#include <vector>
#include "FileReader.h"

using namespace std;

int main()
{
    FileReader reader;
    vector<int> data = reader.read("example2.wav");

    for (vector<int>::iterator i = data.begin(); i != data.end(); ++i)
    {
        //cout << *i << endl;
    }
}

//http://soundfile.sapp.org/doc/WaveFormat/



