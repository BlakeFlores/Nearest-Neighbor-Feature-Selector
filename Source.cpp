#include "nearest_neighbor.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <chrono>

float forward_selection(Problem* prob);
float nearest_neighbor_leave_one_out(Problem* prob, std::vector<int> features_list);
void normalize_data(Problem* prob);
void print_data(Problem* prob);
float backward_elimination(Problem* prob, std::vector<int> features_list);
void print_feature_set(std::vector<int> printme);

int main()
{
	std::fstream inFile;

	std::cout << std::setprecision(3);

	/*
	std::string source_file;
	std::cout << "Type the name of the file to be tested: ";
	std::cin >> source_file;
	*/
	int user_choice;
	std::cout << "\nType the number of the algorithm you want to run:\n ";
	std::cout << "\t1.Forward Selection \n\t2.Backwards Elimination \n"; // \t3.Blake's Special Algorithm\n";
	std::cout << "\t note: Default algoritm is backwards elimination\n";
	std::cin >> user_choice;


	//hardcoded filenames when I'm feeling lazy
	//std::string source_file = "cs_170_small80.txt";
	//std::string source_file = "cs_170_large80.txt";

	//std::string source_file = "cs_170_small39.txt";
	std::string source_file = "cs_170_large39.txt"; // 798/95.5% forward, 

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
		std::cout << "\nThere are " << rows << " Instances with " << columns << " Features in this data set!\n";
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
		
		//Normalize the data
		std::cout << "\n\nPlease wait while I normalize the data....";
		normalize_data(problem);
		std::cout << " Done!\n";

		//create a vector for all columns leave-one-out-nn and fill it (with values for all columns)
		std::vector<int> all_columns;
		for (int i = 1; i < problem->col; i++) //skip 0, the classification column
		{
			all_columns.push_back(i);
		}

		std::cout << "Commencing 'leave-one-out' Nearest neighbor with all " << problem->col << " features!\n ";
		float accuracy_rating = nearest_neighbor_leave_one_out(problem, all_columns);
		std::cout << "Done! With " << accuracy_rating << " accuracy rate!\n\n";


		//start recording time for the algorithm, since we don't care about anything else
		//time code from
		//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/

		auto start_time = std::chrono::high_resolution_clock::now();

		if (user_choice == 1)
		{
			all_columns.clear();
			accuracy_rating = forward_selection(problem);
		}
		else
		{
			accuracy_rating = backward_elimination(problem, all_columns);
		}
		//continuing to use auto since I don't know much about std::chrono (yet)
		auto stop_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop_time - start_time);


		//hardcoded test for small dataset
		//all_columns.push_back(3);
		//all_columns.push_back(5);
		//all_columns.push_back(7);
		//std::cout << "Test " << nearest_neighbor_leave_one_out(problem, all_columns) << std::endl;

		std::cout << "Finished with accuracy rating: " << accuracy_rating << std::endl;
		std::cout << "Final set: ";
		print_feature_set(*problem->final_features_list);
		std::cout << " in " << duration.count() <<  " seconds\n\n";

	}
}

float nearest_neighbor_leave_one_out(Problem* prob, std::vector<int> features_list)
{
	float accuracy_count = 0;
	float* leave_out;

	//for every row
	for (int every_row = 0; every_row < prob->row; every_row++)
	{
		float min_distance = INFINITY; //distance to the closest neighbor
		int min_instance = -1;//index of the closest neighbor
		leave_out = prob->data[every_row];

		//look through all the rows
		for (int rows = 0; rows < prob->row; rows++)
		{
			//skip the left out row to leave it out
			if (prob->data[rows] == leave_out)
			{
				continue;
			}
			float cur_distance = 0; //distance to the neighbor we're examining


			//look at all the columns in this row
			for (std::vector<int>::iterator columns = features_list.begin(); columns!= features_list.end(); columns++)
			{
				//find the distance and add it to the total distance
				cur_distance += pow((prob->data[every_row][*columns] - prob->data[rows][*columns]), 2);
//				cur_distance += pow(leave_out[*columns] - prob->data[rows][*columns], 2);
			}
			cur_distance = sqrt(cur_distance); //finishing the euclidian distance formula

			if (cur_distance < min_distance)
			{
				min_distance = cur_distance;
				min_instance = rows;
			}

		}
		//if our actual classification == our guessed classification (nearest neighbors class)
		if (leave_out[0] == prob->data[min_instance][0])
		{
			accuracy_count += 1;
			 //std::cout << "Success: ";
		}
		//std::cout << prob->data[every_row][0] << " " << prob->data[min_instance][0] << std::endl;
	}
	accuracy_count = accuracy_count / (prob->row );
	return accuracy_count;
}

void print_feature_set(std::vector<int> printme)
{
	std::cout <<  "{";
	for (std::vector<int>::iterator i = printme.begin(); i != printme.end(); i++)
	{
		if (i != printme.begin())
		{
			std::cout << ", ";
		}
		std::cout << *i;
	}
	std::cout << "}";
}

float forward_selection(Problem* prob)
{
	std::vector<int> features_list;

	std::cout << "\n\nBeginning Forward Selection Search: \n";

	bool decrease_flag = false;
	int best_feature = 0;
	float running_accuracy = 0;
	float current_accuracy = 0;
//	float tier_accuracy = 0;

	//add each possible node
	for (int all_features = 0; all_features < prob->col; all_features++)
	{
		float tier_accuracy = 0;
		//check each feature
		for (int feature = 1; feature < prob->col; feature++)
		{
			//check that the selected feature ISN'T already in the vector
			if (std::find(features_list.begin(), features_list.end(), feature) != features_list.end())
			{
				continue;
			}
			else
			{
				//add the feature to the list
				features_list.push_back(feature);
				//check the nearest neighbor leave-one-out error rate
				float tmp_accuracy = nearest_neighbor_leave_one_out(prob, features_list);

				//conforming to output standards
				std::cout << "Using Features: ";
				print_feature_set(features_list);
				std::cout << " accuracy rateing is : " << tmp_accuracy << std::endl;

				//remove the element we just added for next time
				features_list.pop_back();

				//if current error rate is lower, store it (accu
				if (tmp_accuracy > running_accuracy)
				{
					best_feature = feature; //store the current feature to add it for searching later
					running_accuracy = tmp_accuracy;  //store the current best search rate
					tier_accuracy = tmp_accuracy;
				}
				else if (running_accuracy > tier_accuracy)
				{
					tier_accuracy = tmp_accuracy;
					best_feature = feature;

				}
			}
		}

		//store the value of the best single column the first time
		if (all_features == 1)
		{
			std::cout << "\nBest feature is " << best_feature << " With an accuracy rate of : " << running_accuracy << std::endl << std::endl;
			features_list.push_back(best_feature);
			current_accuracy = running_accuracy;
			*prob->final_features_list = features_list;

		}
		//check error rate of the best added feature against previous best rate
		else if (running_accuracy > current_accuracy)
		{
			if (std::find(features_list.begin(), features_list.end(), best_feature) == features_list.end())
			{
				features_list.push_back(best_feature);
			}
			else continue;
			decrease_flag = false; //we're still getting better
			//add the feature to the set (to continue to be grown or not)
//			features_list.push_back(best_feature);

			std::cout << "\nFeature set ";
			print_feature_set(features_list);
			std::cout << " was best with accuracy " << running_accuracy << std::endl;

			//store the current best feature list
			prob->final_features_list->clear();
			*prob->final_features_list = features_list;

			//store the accuracy to ensure we don't get worse later
			current_accuracy = running_accuracy;
		}
		else
		{
			if (decrease_flag == false)
			{
				std::cout << " Warning: Decrease in accuracy found. Continuing in case of local maxima\n\n";
				std::cout << "Feature set ";
				print_feature_set(features_list);
				std::cout << " was best with accuracy " << running_accuracy << std::endl;


				if (std::find(features_list.begin(), features_list.end(), best_feature) == features_list.end())
				{
					features_list.push_back(best_feature);
				}

//				features_list.push_back(best_feature);

				decrease_flag = true;
				current_accuracy = running_accuracy;
			}
			else
			{
//				std::cout << "Exiting search with set "; print_feature_set(*prob->final_features_list); std::cout << "\n";
				return current_accuracy; //if our rate gets worse return the best error rate we have.
			}
		}
		
	}

		return 0;
}

float backward_elimination(Problem* prob, std::vector<int> features_list)
{
	//std::vector<int> features_list;

	std::cout << "Beginning backwards elimination search: \n";

	int current_feature;
	int worst_feature = 0;
	float running_accuracy = 0;
	float current_accuracy = 0;

	//remove one feature at a time from the set until it's empty
	for (int all_features = 1; all_features < prob->col; all_features++)
	{
		float tier_accuracy = 0;
		//remove each feature, one at a time to find the worst one remaining
		for (int remove_feature = 1; remove_feature < prob->col; remove_feature++)
		{
			//save the feature we're about to delete so we can add it back later
			current_feature = features_list.back();
			//remove the feature at the end of the vector
			features_list.pop_back();
			
			//calculate accuracy with one less element
			float tmp_accuracy = nearest_neighbor_leave_one_out(prob, features_list);

			//return it to the beginning!
			features_list.emplace(features_list.begin(), current_feature);

			if (tmp_accuracy > running_accuracy)
			{
				running_accuracy = tmp_accuracy;
				worst_feature = current_feature;
				tier_accuracy = tmp_accuracy; //line added for edge case of never hitting 'else'
			}
			else
			{
				if (tmp_accuracy > tier_accuracy)
				{
					tier_accuracy = tmp_accuracy;
					worst_feature = current_feature;
				}
			}

		}
		//check error rate of the best added feature against previous best rate
		if (running_accuracy > current_accuracy)
		{
			//find the position of the worst feature and remove it
			std::vector<int>::iterator find_delete_pos;
			features_list.erase(std::find(features_list.begin(), features_list.end(), worst_feature));

			//store the accuracy to ensure we don't get worse later
			current_accuracy = running_accuracy;

			//clear the old features list
			prob->final_features_list->clear();
			*prob->final_features_list = features_list;
		}
		else //if no sets are more accurate, remove the worst and continue
		{
			//find the position of the worst feature and remove it
			std::vector<int>::iterator find_delete_pos;
			features_list.erase(std::find(features_list.begin(), features_list.end(), worst_feature));

		}
		std::cout << "Removed " << worst_feature << " to yield accuracy " << tier_accuracy << std::endl;
		std::cout << "Continuing with set "; print_feature_set(features_list); std::cout << std::endl << std::endl;

	}
	std::cout << "Set is empty!\n";
	return current_accuracy;
}


void normalize_data(Problem* prob)
{
	for (int coln = 1; coln < prob->col; coln++)
	{
		//calculate mean for a column
		float mean = 0;
		for (int row = 0; row < prob->row; row++)
		{
			mean += prob->data[row][coln] / prob->row;
		}
//		std::cout << "mean: " << mean << std::endl;


		//calculate the standard deviation of a given column
		float std_dev = 0;
		for (int row2 = 0; row2 < prob->row; row2++)
		{
			std_dev += pow(prob->data[row2][coln] - mean, 2);
		}


		//normalize the data to be between -1 and 1
		for (int row3 = 0; row3 < prob->row; row3++)
		{
			prob->data[row3][coln] = ((prob->data[row3][coln] - mean) / std_dev);
		}
	}

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