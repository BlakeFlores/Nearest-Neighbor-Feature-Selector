# nearest-neighbor
This program does a feature selection using Single Nearest Neighbor. It takes in an input file of data which is formatted into NxM rows, 
and whose data is space delinated. It was originally designed for IEEE floating point style numbers, but any *numeric* data should work.

It's written in C++, using nothing but standard libraries.

# Dependancies

This is written in C++

# Installation:

To install, simply downlaod the source.cpp, nearest_neighbor.cpp and nearest_neighbor.h files, and compile them together using your
favorite compiler.

# Running

To run the program, utilize whatever executable/object file is output by your compiler. Once the program begins, it will ask for the path
to a source file containing formatted data. From there, it will ask whether to do forward selection or backward eliminaiton. Finally, it will
determine the most optimal set of features for the given data to create a modeling algorithm with.
