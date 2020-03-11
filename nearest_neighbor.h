#pragma once
#include <vector>
#include <fstream>

class Problem
{
public:
	float** data;
	int col, row;
	Problem(int col, int row);
	std::vector<int>* final_features_list;
};


