#pragma once
#include <string>
#include <vector>

struct dataStructure {
    long time;
    long id;
    std::string value;
};

class csvLoader {
public:
    static std::vector<dataStructure> LoadCSV(std::string* filename);
};