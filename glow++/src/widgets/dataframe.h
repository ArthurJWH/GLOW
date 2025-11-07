#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <string>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, std::vector<std::string>> readCsv(const std::string &filename);

#endif // DATAFRAME_H
