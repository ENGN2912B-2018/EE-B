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

  - Coming Soon

## UML Diagram:

  - Coming Soon
  
# Interface Descriptions

## Graphical Interface

  - Coming Soon

## Data Visualization

  - Coming Soon

## Exception Handling

  - Coming Soon

# Testing and Evaluation

## Operating system(s) and software compiler/library version(s) tested

  - Coming Soon

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
