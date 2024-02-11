#include "../include/csvLoader.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<dataStructure> csvLoader::LoadCSV(std::string* filename)
{
    std::cout << "Start!" << std::endl;
    // Variables
    std::ifstream target(*filename);
    std::string text;
    std::vector<std::string> csvFile;

    // Seperate file into vector of string lines
    while (std::getline(target, text, '\n')) {
        csvFile.push_back(text);
    }

    // Seperate each line by commas
    std::vector<dataStructure> dataFile;
    for (int i = 0; i < csvFile.size(); i++) {
        // Convert each line into new stream
        std::stringstream line(csvFile[i]);
        int column = 0;
        dataStructure structure;
        // Add to structure depending on column
        while (std::getline(line, text, ',')) {
            // Add to structure per column
            switch (column) {
            case 0:
                structure = *new (dataStructure);
                structure.time = std::stol(text);
                break;
            case 1:
                structure.id = std::stol(text);
                break;
            case 2:
                structure.value = text;
                break;
            default:
                std::cout << "Column greater than 3 detected!" << std::endl;
                break;
            }
            // Increment column
            column++;
        }
        // Add finished data to structure
        dataFile.push_back(structure);
    }
    // Close file stream
    target.close();
    std::cout << "Done!" << std::endl;
    return dataFile;
}