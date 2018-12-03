#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <vector>

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
    char * ChunkID = new char[4];
    char * ChunkSize = new char[4];
    char * Format = new char[4];
    char * SubChunk1ID = new char[4];
    char * SubChunk1Size = new char[4];
    char * AudioFormat = new char[2];
    char * NumChannels = new char[2];
    char * SampleRate = new char[4];
    char * ByteRate = new char[4];
    char * BlockAlign = new char[2];
    char * BitsPerSample = new char[2];
    char * SubChunk2ID = new char[4];
    char * SubChunk2Size = new char[4];
    char * sample1 = new char[4];

    typedef unsigned char u8;  // in case char is signed by default on your platform
    typedef char u9;

    ifstream ifstr(fileName.c_str(), ifstream::binary);
    ifstr.read(ChunkID,4);
    cout << ChunkID << endl;

    ifstr.read(ChunkSize,4);
    unsigned num = ((u8)ChunkSize[3] << 24) | ((u8)ChunkSize[2] << 16) | ((u8)ChunkSize[1] << 8) | (u8)ChunkSize[0];
    cout << num << endl;

    ifstr.read(Format,4);
    cout << Format << endl;

    ifstr.read(SubChunk1ID,4);
    //num = ((u8)SubChunk1ID[0] << 24) | ((u8)SubChunk1ID[1] << 16) | ((u8)SubChunk1ID[2] << 8) | (u8)SubChunk1ID[3];
    cout << SubChunk1ID << endl;

    ifstr.read(SubChunk1Size,4);
    num = ((u8)SubChunk1Size[3] << 24) | ((u8)SubChunk1Size[2] << 16) | ((u8)SubChunk1Size[1] << 8) | (u8)SubChunk1Size[0];
    cout << num << endl;

    ifstr.read(AudioFormat,2);
    num = 0;
    num = (((u8)AudioFormat[1] << 8) | (u8)AudioFormat[0]);
    cout << num << endl;

    ifstr.read(NumChannels,2);
    num = 0;
    num = (((u8)NumChannels[1] << 8) | (u8)NumChannels[0]);
    cout << num << endl;

    ifstr.read(SampleRate,4);
    num = (((u8)SampleRate[3] << 24) | ((u8)SampleRate[2] << 16) | ((u8)SampleRate[1] << 8) | (u8)SampleRate[0]);
    cout << num << endl;

    ifstr.read(ByteRate,4);
    num = (((u8)ByteRate[3] << 24) | ((u8)ByteRate[2] << 16) | ((u8)ByteRate[1] << 8) | (u8)ByteRate[0]);
    cout << num << endl;

    ifstr.read(BlockAlign,2);
    num = 0;
    num = (((u8)BlockAlign[1] << 8) | (u8)BlockAlign[0]);
    cout << num << endl;

    ifstr.read(BitsPerSample,2);
    num = 0;
    num = (((u8)BitsPerSample[1] << 8) | (u8)BitsPerSample[0]);
    cout << num << endl;

    ifstr.read(SubChunk2ID,4);
    cout << SubChunk2ID << endl;

    ifstr.read(SubChunk2Size,4);
    num = ((u8)SubChunk2Size[3] << 24) | ((u8)SubChunk2Size[2] << 16) | ((u8)SubChunk2Size[1] << 8) | (u8)SubChunk2Size[0];
    cout << num << endl;
    int msize = num;

    vector<int> data;
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
