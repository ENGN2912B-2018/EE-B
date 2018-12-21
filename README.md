# Feedback Identification Algorithm for Embedded Applications
This repository will house the final project of Embedded Electronics Group B.

## Team Members:
Domenic Ferri - Contact Email: domenic_ferri@brown.edu
Alex Cannan - Contact Email: alex_cannan@brown.edu 

## Course:
Fall 2018 ENGN2912B - Scientific Programming in C++

Data Submitted: December 2018

# Introduction

## Project Overview:

When a microphone is in the presence of a speaker, this creates a feedback loop. This feedback loop
can create adverse artifacts, in the form of tonal feedback, based on the frequency response of the
devices and the geometry of the setup. We believe that a system can be developed for embedded use
that will be able to identify feedbacking frequencies using a sophisticated method and eliminate them.

There are many reasons of motivation for our project. This first is that standard feedback eliminators use a 
threshold magnitude in order to identify feedback. This means that if the amplitude of a particular frequency
is above a choosen amount, then it is labeled as feedback and removed. The problem is that this leads to alot
of inacuracies and plenty of healthy musical frequiencies getting removed as well. The reason is that amplitude
alone is not enough information to know if a frequency is undergoing feedback or not. Instead, a good feedback
identification algorithm must also consider the rate of change in amplitude from sample to sample (swelling),
the harmonic structure, spectral width, and lifespan of the offending frequencies. Our algorithm assigns a probabilty
to a frequency that represents how likely it is that it is undergoing feedback based on the following 5 checks:

#### (1) SNR Check

- This component will exclude any peaks below a certain signal to noise threshold

#### (2) Swell Check

- This component will assign a probability to frequencies that are increasing over time, indicating a feedback loop

#### (3) Harmonic Check

- This component will check if frequencies with a non-zero probability have harmonics--feedback tones should be isolated, while instrument tones will have harmonics

#### (4) Spectral Width Check

- This component will look at the frequencies in close proximity to the budding feedback and identify if they are growing at a similar rate. This would indicate wide-band feedback, and must be dealt with a wider band-stop filter

#### (5) Sustain Check

- This component will keep tabs on possible feedback peaks, and then increase their probability if they're sustained for an extended period. It is possible that feedback can reach a maximum intensity based on the external system, so this will take that into account.

By taking into consideration all these factors, the likelyhood of errors is much less than using only a single 
threshold. 

Feedback eliminators that have these more advanced identification algorithms are often unaccessable due to price
or portability. If a person wants to record feedbackless audio, they would likely have to go to a recording studio
or buy and setup expensive and complex exquipment. Our feedback identifier is built on and operates on a Respberry Pi
making it a very inexpensive and portable feedback eliminator. The Raspberry Pi proves convient with its audio ports
that allow connecting a headset and microphone to record, playback, save, and remove feedback with a powerful algorithm
from the confort of thier own home and without breaking the bank.

## Goals and Objectives:

#### Goal 1: 

- The ability to read in and parse a .wav file for header and sound data. (Completed)
  
#### Goal 2: 

- The ability to take the FFT of sound data samples from a file. (Completed)
  
#### Goal 3:

- The ability to process the FFT to identify offending frequencies. (Completed)
  
#### Goal 4:

- GUI Interface that allows the user to input algorithm variables, select a file for processing, and output any error messages. (Completed)

#### Goal 5:

- To display a plot of the results: A spectogram showing the frequencies identifies as feedback. (Completed)

#### Goal 6:

- The ability to eliminate the feedback. (Not Completed)

#### Goal 7:

- The ability to use the identifaction and elimination algorithims on live microphone input from the Pi. (Not Completed)

# Software Architectual Design

The software contains the following files:

- FileReader.cpp and FileReader.h: Responsable for reading and parsing a .wav file. Input a file location and outputs an int vector containing the audio data.
- FFTAnalyzer.h: Uses the kiss_fft130 FFT library to compute the FFT of the audio data. Takes as input an int vector and outputs a 2D vector containing the FFT for each sample.
- FeedbackID.cpp and FeedbackID.h: Impliments the five feedback checks and assigns a feedback liklihood probabilty to each frequency for every sample. Takes as input the 2D FFT vector and outputs at 2D vector of probabilities.
- gnuplot_i.hpp: C++ interface for gnuplot.
- analyzerwindow.cpp and analyzerwindow.h: Contains the QT GUI implimentation.
- main.cpp: Calls the QT interface
- WorkingFeedbackGUI.pro: qmake file to compile the project
- kiss_fft130: Included external library that computes the FFT.

## Required External Libraties:

 - Kiss_fft130 (included)
 - Boost Version 1.63.0
 - GNUPlot
 - QT Version 5.7.0

## Functional Block Diagram:

![alt text](EE-B/images/Functional.PNG?raw=true "Functional Block Diagram")

## UML Diagram:

![uml](https://user-images.githubusercontent.com/43046820/50324573-aecc7900-04ad-11e9-99a5-5a35193e6f62.PNG)
  
# Interface Descriptions

## Graphical Interface

  - The graphical interface has been created using QT Version 5.7.0. It consist of a simple GUI that allows the user to browse for a .wav file, select the weights for each of the checks, choose the analysis period, and show a progress bar. It also consists of two buttons, one that calls the feedback identification algorithm with the chosen user inputs, and one that closes the program. If the file path inputted does not exist, or it is not of proper .wav format, then an error message is shown, and the user will be able to select another file. The code structure of the GUI makes it easy to add future features as they are implemented.  

## Data Visualization

  - Using the Gnuplot library and a Gnuplot C++ interface, we are able to plot the results of the feedback identification. The plotted results are in the form of a spectrogram that plots the FFT spectrum as sample number (x-axis), verses frequency (y-axis), verses frequency magnitude (z-axis in color). Over the spectrogram, red highlights are placed over the spectrogram plot that shows how high the probabilities of feedback for each frequency component are. The reader the component is, the higher the probability that the frequency is undergoing feedback. This allows the user to see both the spectrogram of the audio data and feedback probabilities in one plot.

## Exception Handling

  There are two areas of concern where errors can occur that prevent the algorithm from continuing as normal. The first of which dealing with the input .wav file. If an error occurs while reading and processing the file, the file reader retunes the data vector containing a single integer representing the error code to the analyzerwindow which outputs the corresponding error message in a new window. In all cases, the program will not crash and the user will be able to try new inputs. The possible errors regarding the file reader are as follows:

 - Error code 1: The file does not exist or cannot be found.

 - Error code 2: The file is too small to be a .wav file. Occurs if the file size is less than 44 bytes.
 
 - Error code 3, 4 and 7: The file header does not contain the bytes "RIFF", "fmt", or "data". All wav files should contain these three bytes. In the .wav format, the bytes following these indicators contain valuable information regarding sample rate, sample size, compression, format, and more. We have also found out that sometimes processing wav files in other audio software can sometimes add to move around the header values. Because this is difficult to predict, we do not support files with "RIFF", "fmt", or "data" not in the correct position, which is why we check all three and not just one.

 - Error code 5: The header contains a byte that represents file compression. If the byte reads one, then the file is uncompressed, if the byte is not 1, the file cannot be processed.

 - Error code 6: The header contains a byte that represents the sample format. If the byte reads one, then the file is a mono .wav file, meaning the audio is in the form of a single stream. Some dual channel .wav files can have two channels with alternating samples, which we do not support. If the byte is not read 1, the file cannot be processed.

In all of these situations, the program alerts the user and asks for another file.

The other area where errors can occur is when calculating the probability of feedback for each frequency component using the five checks. Errors can occur when the threshold values and other check attributes are out of range or do not make sense. Originally, we wanted these values to be adjustable from the GUI, however we found a set of constant values that work well for all files. However, incase these values are adjusted within the code, a set of checks were implemented to ensure that the values make sense and will not cause nonsensical results. When these range errors occur, the algorithm proceeds and finishes are normal but a warning is displayed telling the user which value is out of range and that the results may not make sense. 

# Testing and Evaluation

## Operating system(s) and software compiler/library version(s) tested

### Raspberry Pi:

  - Raspberry Pi Version 3 Model B+
  - Raspbian Operating System
  - Compliler Version (gcc and g++): 6.3.0
  - QT Version: 5.7.1
  - Boost Version: 1.63.0
  - Gnuplot Version: 5.0

### CCV:

  - Compiler Version (gcc and g++): 7.2
  - QT Version: 5.7.0
  - Boost Version: 1.63.0
  - Gnuplot Version: 4.6

## Description of functional testing to date

  - Coming Soon

## Instructions for compiling and running the software

  - Coming Soon

# Conclusions

## Major Accomplishments

  - Coming Soon

## Results of the software development effort

  - Coming Soon

## Level of Success

  - Coming Soon

# Future Work

## Potential future software expansions and software features

  - Coming Soon

# Author Contributions

  - Coming Soon
