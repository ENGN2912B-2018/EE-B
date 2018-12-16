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
  unsigned getsamplerate();
		
 protected:
 private:
  unsigned sampleRate;
};

#endif // FILEREADER_H
