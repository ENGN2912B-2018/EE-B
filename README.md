# Feedback Elimination Algorithm for Embedded Applications
This repository will house the final project of ENGN 2912B team EE-B. It will contain a framework to build and optimize the algorithm, which can then be extracted for external use.

## Design Philosophy

The purpose of this project is to develop an algorithm that will be able to detect and report on budding feedback in a live input. This development will involve constructing a test environment within a Raspberry Pi 3, which can intake signals from either files, and eventually, a live input. These incoming signals will be split into buffers, which will then be processed by an FFT function from an external library. Once processed, the spectral information of all the buffers will be inputs to the feedback identification algorithm. The algorithm will then generate possible feedback frequencies by utilizing several identification techniques, which will be assigned weights within the algorithm. These weights will eventually be adjusted for speed and accuracy of feedback detection. Once a sufficient algorithm is produced, the project will then apply band-stop filters to the identified frequencies, and route these back to the input of the program. This test will exemplify a real-life feedback situation, one that changes dynamically as filtering is applied.

The output of this project is meant to be an extractable feedback detection algorithm for use in embedded applications. Since embedded devices usually have hardware-accelerated processes for common DSP processes (FFT, Filtering, Sampling), our project will focus on yielding portable code for feedback detection, while leaving the rest of the development up to the embedded engineers.

## Build Instructions

TBD

## Usage Documentation

### Feedback Identification Components

