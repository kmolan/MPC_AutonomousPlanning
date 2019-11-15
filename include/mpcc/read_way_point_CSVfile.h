#pragma once

#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace mpcBlock {

    std::vector <std::vector<float>> read_way_point_CSVfile(std::string filename) {
        // modified from https://thispointer.com/how-to-read-data-from-a-csv-file-in-c/
        std::string delm = ",";
        std::ifstream file(filename);
        std::vector <std::vector<float>> dataVector_out;
        std::string line = "";
        std::vector<float> dataVector0;
        std::vector<float> dataVector1;
        std::vector<float> dataVector2;
        // Iterate through each line and split the content using delimeter

        while (getline(file, line)) {
            std::vector <std::string> vec;
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
}