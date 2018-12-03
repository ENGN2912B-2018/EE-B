#ifndef FILEREADER_H
#define FILEREADER_H
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


class FileReader
{
    public:
        FileReader();
        ~FileReader();
        vector<int> read(string fileName);

    protected:

    private:
};

#endif // FILEREADER_H
