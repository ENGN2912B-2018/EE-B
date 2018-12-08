# Feedback Elimination Algorithm for Embedded Applications
This repository will house the final project of ENGN 2912B team EE-B. It will contain a framework to build and optimize the algorithm, which can then be extracted for external use.

## Design Philosophy

The purpose of this project is to develop an algorithm that will be able to detect and report on budding feedback in a live input. This development will involve constructing a test environment within a Raspberry Pi 3, which can intake signals from files, and eventually, a live input. These incoming signals will be split into buffers, which will then be processed by an FFT function from an external library. Once processed, the spectral information of all the buffers will be inputs to the feedback identification algorithm. The algorithm will then generate possible feedback frequencies by utilizing several identification techniques, which will be assigned weights within the algorithm. These weights will eventually be adjusted for speed and accuracy of feedback detection. Once a sufficient algorithm is produced, the project will then apply band-stop filters to the identified frequencies, and route these back to the input of the program. This test will exemplify a real-life feedback situation, one that changes dynamically as filtering is applied.

The output of this project is meant to be an extractable feedback detection algorithm for use in embedded applications. Since embedded devices usually have hardware-accelerated processes for common DSP processes (FFT, Filtering, Sampling), our project will focus on yielding portable code for feedback detection, while leaving the rest of the development up to the embedded engineers.

## Build Instructions

After cloning the repository, you can build with CMake. From the root directory (EE-B), input:

`mkdir build
cd build
cmake CMAKE_CXX_COMPILER=g++ ..
make`

And to run:

`./RemoveFeedback`

## Usage Documentation

### Feedback Identification Components

The individual feedback identifaction components will be listed here with a brief description

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
