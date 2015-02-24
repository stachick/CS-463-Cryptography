//stenography.cpp
//Shane Tachick
//February 23, 2015
//Program that will hopefully hide data instide of an img file

#include <iostream>
using std::endl;
using std::cout;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;

//a tuple of color nodes, 3 ints, red green and blue values in that order
typedef tuple<int, int, int> redGreenBlue;

bool loadVector(vector<string> &wordlist)
{
	ifstream infile("tinypix.ppm");
	string current;
	while (true)
	{
		getline(infile, current);
		if (!infile)
		{
			if (infile.eof())
			{
				cout << "----------------------------" << endl;
				cout << "end of file reached, groovy." << endl;
				return true;
			}
			else
			{
				cout << "danger will robinson, danger!" << endl;
				return false;
			}
		}
		wordlist.push_back(current);
	}
}

void sanitizeInput(vector<string> test, vector<vector<redGreenBlue>> &grid, string &filetype, int &x, int &y, int &colorValue)
{
	string temp;

	filetype = test[0];

}

int main()
{
	string filetype;
	int x;
	int y;
	int colorValue;

	vector <string> test;
	//an x-y vector containing the red, gree, blue values.
	vector<vector<redGreenBlue>> grid;

	loadVector(test);

	for (int i = 0; i < test.size(); i++)
	{
		cout << test[i] << endl;
	}
	
	cout << test[0] << endl;
	cout << test[1] << endl;
	cout << test[2] << endl;
	cout << test[3] << endl;

	sanitizeInput(test, grid, filetype, x, y, colorValue);



	return 0;
}