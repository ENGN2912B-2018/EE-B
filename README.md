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

## Required External Libraties:

## Functional Block Diagram:

## UML Diagram:

# Interface Descriptions

## Graphical Interface

## Data Visualization

## Exception Handling

# Testing and Evaluation

## Operating system(s) and software compiler/library version(s) tested

## Description of functional testing to date

## Instructions for compiling and running the software

# Conclusions

## Major Accomplishments

## Results of the software development effort

## Level of Success

# Future Work

## Potential future software expansions and software features

# Author Contributions



## Design Philosophy

The purpose of this project is to develop an algorithm that will be able to detect and report on budding feedback in a live input. This development will involve constructing a test environment within a Raspberry Pi 3, which can intake signals from files, and eventually, a live input. These incoming signals will be split into buffers, which will then be processed by an FFT function from an external library. Once processed, the spectral information of all the buffers will be inputs to the feedback identification algorithm. The algorithm will then generate possible feedback frequencies by utilizing several identification techniques, which will be assigned weights within the algorithm. These weights will eventually be adjusted for speed and accuracy of feedback detection. Once a sufficient algorithm is produced, the engineer can then apply band-stop filters to the identified frequencies, and route these back to the input of the program. This test will exemplify a real-life feedback situation, one that changes dynamically as filtering is applied.

The output of this project is meant to be an extractable feedback detection algorithm for use in embedded applications. Since embedded devices usually have hardware-accelerated processes for common DSP processes (FFT, Filtering, Sampling), our project will focus on yielding portable code for feedback detection, while leaving the rest of the development up to the embedded engineers.

## Build Instructions

After cloning the repository, you can build with CMake. From the root directory (EE-B), input:

```
mkdir build

cd build

cmake CMAKE_CXX_COMPILER=g++ ..

make
```

And to run:

`./RemoveFeedback`
