#include "nearest_neighbor.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>


bool load_data(float** data_array);
void normalize_data(Problem* prob);
void print_data(Problem* prob);

int main()
{
	std::fstream inFile;


	/*
	std::string test_filename;
	std::cout << "Type the name of the file to be tested: ";
	std::cin >> test_filename;
	*/

	/*
	int user_choice;
	std::cout << "\nType the number of the algorithm you want to run:\n ";
	std::cout << "\t1.Forward Selection \n\t2.Backwards Elimination \n\t3.Blake's Special Algorithm\n";
	std::cin >> user_choice;
	*/
	std::string source_file = "cs_170_small80.txt";
	inFile.open(source_file);
	if (!inFile)
	{
		std::cout << "Error, unable to open file\n";
		exit(1);
	}
	else
	{
		int columns = 0;
		int rows = 0;
		std::string count_rows_colns;

		/*
		(partial) credit for counting rows/columns goes to
		https://stackoverflow.com/questions/43768877/how-to-know-its-the-end-of-the-line-in-c-reading-file
		*/
		while (inFile >> count_rows_colns)
		{
			char c = inFile.get();
			if (c == '\n')
				rows++;

			if (rows == 1)
				columns++;
		}
		std::cout << rows << " Instances with " << columns << " Features in this data set!\n";
		Problem* problem = new Problem(columns, rows);

		//restart at the beginning of the file
		inFile.clear();
		inFile.seekg(0, inFile.beg);

		//fill the data array with data from our file
		int cur_row=0, cur_coln = 0;
		std::string piece_of_data;
		while (inFile >> piece_of_data)
		{
			problem->data[cur_row][cur_coln] = std::stof(piece_of_data);
			cur_coln++;

			if (cur_coln == columns)
			{
				cur_coln = 0;
				cur_row++;
			}
		}

		normalize_data(problem);
//		print_data(problem);

	}
}


bool load_data(float** data_array)
{
	return true;
}


void normalize_data(Problem* prob)
{
	for (int coln = 0; coln < prob->col; coln++)
	{
		//calculate mean
		float mean = 0;
		for (int row = 0; row < prob->row; row++)
		{
			mean += prob->data[coln][row] / prob->row;
		}
		std::cout << "mean: " << mean << std::endl;

		float std_dev = 0;
		for (int row2 = 0; row2 < prob->row; row2++)
		{
			std_dev += pow(prob->data[coln][row2] - mean, 2);
		}
		std::cout << "std_dev: " << std_dev << std::endl;


		for (int row3 = 0; row3 < prob->row; row3++)
		{
			prob->data[coln][row3] = (prob->data[coln][row3] - mean) / std_dev;
		}
	}

	/*
	for (int row = 0; row < prob->row; row++)
	{
		//calculate the mean
		float mean = 0;
		for (int col = 0; col < prob->col; col++)
		{
			mean += prob->data[row][col]/ prob->row;
		}
		std::cout << "mean" << mean << std::endl;


		//calculate the standard deviation
		float std_dev = 0;
		for (int i = 0; i < prob->row; i++)
		{
			std_dev += pow(prob->data[row][i] - mean, 2);
		}

		std_dev = std_dev / prob->row;
		std_dev = sqrt(std_dev);

		std::cout << "std_dev: " << std_dev << std::endl;

		//normalize
		for (int j = 0; j < prob->row; j++)
		{
			prob->data[row][j] = (prob->data[row][j] - mean) / std_dev;

//			std::cout << prob->data[row][j] << " ";
		}
//		std::cout << std::endl;
	}
	*/
}


void print_data(Problem* prob)
{
	for (int i = 0; i < prob->row; i++)
	{
		for (int j = 0; j < prob->col; j++)
		{
			std::cout << prob->data[i][j] << " ";
		}
		std::cout << "\n";
	}
}
