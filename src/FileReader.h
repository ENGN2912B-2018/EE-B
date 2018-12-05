#ifndef FILEREADER_H
#define FILEREADER_H
#include <iostream>
#include <vector>

using namespace std;


class FileReader
{
    public:
        FileReader();
        ~FileReader();
        vector<int> read(string filename);

    protected:

    private:
};

#endif // FILEREADER_H
