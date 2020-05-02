# nearest-neighbor
This program does a feature selection using Single Nearest Neighbor. It takes in an input file of numeric data which is formatted accordingly (see dependancies below). This program then noramlizes all of the data to a range of between -1 and 1. Finally, It uses a single nearest neighbor algorithm to determine which combination of features can best predict the classification of future data. The output of this program is the most optimal set of features for building a nearest neighbor classifier.


This does have the drawback of being a supervised selector, labeled training data is required for this program to work.

# Dependancies

This is written in C++, and uses the standard libraries. 

It is however dependant on a correctly formatted input file. 

The first column should be the label of each element. it should be some numeric value to identify the category of the training data

>i.e.store 1,2,3... in the first column, where 1 = cats, 2 = dogs 3 = birds...

Each feature should be purely numeric data, and should be space dileniated. The current setup reads each feature element as a string and converts it to a float before normalizing, so any number format supported by [std::stof](http://www.cplusplus.com/reference/string/stof/) should work as input data.

Finally, each element/row of data should be newline sepaarated.

# Installation:

To install, simply downlaod the source.cpp, nearest_neighbor.cpp and nearest_neighbor.h files, and compile them together using your
favorite compiler.

# Running

To run the program, utilize whatever executable/object file is output by your compiler. Once the program begins, it will ask for the path
to a source file containing formatted data. From there, it will ask whether to do forward selection or backward eliminaiton. Finally, it will
determine the most optimal set of features for the given data to create a modeling algorithm with, and display the optimal subset of features.


# TODO

>modify this to use k-nearest-neighbors (trivial, it's just another loop and some more data tracking)

>add k-selector function to find optimal number of neighbors for k-n-n (gonna be hard, also fun!)

>improve overall speed. Probably through some use of multithreading some of the loops to simultaneously process them instead of sequentially.


If you have any questions at all, feel free to email me at blake.i.flores@gmail.com
