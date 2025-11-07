#include "dataframe.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(line);
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::unordered_map<std::string, std::vector<std::string>> readCsv(const std::string &filename){
    std::unordered_map<std::string, std::vector<std::string>> data;

    std::ifstream file(filename);
    std::string line;

    // header
    std::getline(file, line);
    std::vector<std::string> keys = splitLine(line);
    for (std::string &key : keys) data[key] = {};

    // values
    std::vector<std::string> values;
    while (std::getline(file, line)){
        values = splitLine(line);
        for (int i = 0; i < keys.size(); i++){
            data[keys[i]].push_back(values[i]);
        }
    }

    return data;
}
