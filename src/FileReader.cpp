#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

FileReader::FileReader()
{
    //ctor
}

FileReader::~FileReader()
{
    //dtor
}

vector<int> FileReader::read(string fileName)
{
    char * ChunkID = new char[5];
    char * ChunkSize = new char[4];
    char * Format = new char[5];
    char * SubChunk1ID = new char[5];
    char * SubChunk1Size = new char[4];
    char * AudioFormat = new char[2];
    char * NumChannels = new char[2];
    char * SampleRate = new char[4];
    char * ByteRate = new char[4];
    char * BlockAlign = new char[2];
    char * BitsPerSample = new char[2];
    char * SubChunk2ID = new char[5];
    char * SubChunk2Size = new char[4];
    char * sample1 = new char[4];

    vector<int> data;

    streampos begin, end;

    typedef unsigned char u8;  // in case char is signed by default on your platform
    typedef char u9;
    
    ifstream ifstr(fileName.c_str(), ifstream::binary);

    //// ERROR CHECKING ----------------------------------------------------
    if(!ifstr) { // error code 1
      cout << "FILE ERROR: File not found" << endl; 
      data.push_back(1);
      return data;
    }
    //// ERROR CHECKING ----------------------------------------------------

    begin = ifstr.tellg();
    ifstr.seekg(0, ios::end);
    end = ifstr.tellg();
    cout << "Filesize is: " << end-begin << " bytes." << endl;
    ifstr.seekg(0, ios::beg);

    //// ERROR CHECKING ----------------------------------------------------
    if(end-begin < 44) { // error code 2
      cout << "FILE ERROR: This file is too small to be a wave file";
      data.push_back(2);
    }
    //// ERROR CHECKING ----------------------------------------------------

    ifstr.read(ChunkID,4);
    ChunkID[4] = '\0';
    cout << "ChunkID: " << ChunkID << endl;

    //// ERROR CHECKING ----------------------------------------------------
    if(strcmp(ChunkID, "RIFF")) { // error code 3
      cout << "FILE ERROR: Wave File Header not found. The input file may not be a .wave file." << endl;
      data.push_back(3);
    }
    //// ERROR CHECKING ----------------------------------------------------

    ifstr.read(ChunkSize,4);
    unsigned num = ((u8)ChunkSize[3] << 24) | ((u8)ChunkSize[2] << 16) | ((u8)ChunkSize[1] << 8) | (u8)ChunkSize[0];
	cout << "ChunkSize: " << num << endl;

    ifstr.read(Format,4);
    Format[4] = '\0';
    cout << "Format: " << Format << endl;

    ifstr.read(SubChunk1ID,4);
    SubChunk1ID[4] = '\0';
    cout << "SubChunkID: " << SubChunk1ID << endl;

    //// ERROR CHECKING ----------------------------------------------------
    if(strcmp(SubChunk1ID, "fmt ")) { // error code 4
      cout << "FILE ERROR: Wave File Header not found. The input file may not be a .wave file." << endl;
      data.push_back(4);
    }
    //// ERROR CHECKING ----------------------------------------------------

    ifstr.read(SubChunk1Size,4);
    num = ((u8)SubChunk1Size[3] << 24) | ((u8)SubChunk1Size[2] << 16) | ((u8)SubChunk1Size[1] << 8) | (u8)SubChunk1Size[0];
    cout << "SubChunkSize: " << num << endl;

    ifstr.read(AudioFormat,2);
    num = 0;
    num = (((u8)AudioFormat[1] << 8) | (u8)AudioFormat[0]);
    cout << "AudioFormat: " << num << endl;

    //// ERROR CHECKING ----------------------------------------------------
    if(num != 1) { // error code 5
      cout << "FORMAT ERROR: Wave Files must be uncompressed." << endl;
      data.push_back(5);
    }
    //// ERROR CHECKING ----------------------------------------------------

    ifstr.read(NumChannels,2);
    num = 0;
    num = (((u8)NumChannels[1] << 8) | (u8)NumChannels[0]);
    cout << "NumChannels: " << num << endl;

    //// ERROR CHECKING ----------------------------------------------------
    if(num != 1) { // error code 6
      cout << "FORMAT ERROR: This reader only supports mono wavefiles" << endl;
      data.push_back(6);
    }
    //// ERROR CHECKING ----------------------------------------------------

    ifstr.read(SampleRate,4);
    num = (((u8)SampleRate[3] << 24) | ((u8)SampleRate[2] << 16) | ((u8)SampleRate[1] << 8) | (u8)SampleRate[0]);
    sampleRate = num;
	cout << "SampleRate: " << num << endl;

    ifstr.read(ByteRate,4);
    num = (((u8)ByteRate[3] << 24) | ((u8)ByteRate[2] << 16) | ((u8)ByteRate[1] << 8) | (u8)ByteRate[0]);
    cout << "ByteRate: " << num << endl;

    ifstr.read(BlockAlign,2);
    num = 0;
    num = (((u8)BlockAlign[1] << 8) | (u8)BlockAlign[0]);
    cout << "BlockAlign: " << num << endl;

    ifstr.read(BitsPerSample,2);
    num = 0;
    num = (((u8)BitsPerSample[1] << 8) | (u8)BitsPerSample[0]);
    cout << "BitsPerSample: " << num << endl;

    ifstr.read(SubChunk2ID,4);
    SubChunk2ID[4] = '\0';
    cout << "SubChunk2ID: " << SubChunk2ID << endl;

    //// ERROR CHECKING ----------------------------------------------------
    if(strcmp(SubChunk2ID, "data")) { // error code 7
      cout << "FILE ERROR: Wave File Header not found. The input file may not be a .wave file." << endl;
      data.push_back(7);
    }
    //// ERROR CHECKING ----------------------------------------------------

    ifstr.read(SubChunk2Size,4);
    num = ((u8)SubChunk2Size[3] << 24) | ((u8)SubChunk2Size[2] << 16) | ((u8)SubChunk2Size[1] << 8) | (u8)SubChunk2Size[0];
    cout << "SubChunk2Size: " << num << endl;

    int msize = num;

    //// ERROR CHECKING ----------------------------------------------------
    if (data.size() > 0) {
        return data;
    } // if errors detected, abort with error codes
    //// ERROR CHECKING ----------------------------------------------------


    char * sample = new char[4];
    int sampleInt;
    for(unsigned i = 0; i < msize; i+=2)
    {
        ifstr.read(sample,2);
        sampleInt = 0;
        sampleInt = (((u9)sample[1] << 8) | (u9)sample[0]);
        data.push_back(sampleInt);
    }
    return data;
}

unsigned FileReader::getsamplerate()
{
	return sampleRate;
}
