#include "nearest_neighbor.h"

Problem::Problem(int cols, int rows)
{
	this->data = new float* [rows];
	//create the structure for the stored data
	for (int i = 0; i < rows; i++)
	{
		this->data[i] = new float[cols];
	}

	this->col = cols;
	this->row = rows;
}