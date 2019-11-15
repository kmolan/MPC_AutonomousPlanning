#pragma once

using namespace std;
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
vector<vector<float>> read_way_point_CSVfile(string filename){
    // modified from https://thispointer.com/how-to-read-data-from-a-csv-file-in-c/
    string delm = ",";
	ifstream file(filename);
    vector<vector<float>> dataVector_out;
	string line = "";
    vector<float> dataVector0;
    vector<float> dataVector1;
    vector<float> dataVector2;
	// Iterate through each line and split the content using delimeter
	while (getline(file, line)) {
        vector<string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delm));
		dataVector0.push_back(stof(vec[0]));
        dataVector1.push_back(stof(vec[1]));
        dataVector2.push_back(stof(vec[2]));
	}
    dataVector_out.push_back(dataVector0);
    dataVector_out.push_back(dataVector1);
    dataVector_out.push_back(dataVector2);
	// Close the File
	file.close();
 
	return dataVector_out;
}