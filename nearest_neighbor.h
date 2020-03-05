#pragma once
#include <vector>
#include <fstream>

struct feature
{
	std::vector<float>* attribute_features; //a single row
};



class feature_selection
{
	std::vector<feature>* feature_list; //list of rows
};




class Problem
{
public:
	float** data;
	int col, row;
	Problem(int col, int row);
};


