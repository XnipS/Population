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
                // Parse data values
                if (text == "Connect") {
                    structure.value = 1;
                } else if (text == "Disconnect") {
                    structure.value = 0;
                } else if (text == "STARTED") {
                    structure.value = -1;
                } else {
                    std::cout << "Unknown data value detected!" << std::endl;
                    structure.value = -1;
                }
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

void csvLoader::RepairData(std::vector<dataStructure>* data)
{
    // Saved users
    dictionary users;
    // Loop through csv rows
    for (int i = 0; i < data->size(); i++) {
        // Get next row
        long* nextId = &(*data)[i].id;
        int* val = &(*data)[i].value;
        bool trueVal;
        // Check if row is STARTER
        if (*nextId == 0) {
            continue;
        }
        // Check if value is valid
        switch (*val) {
        case 0:
            trueVal = false;
            break;
        case 1:
            trueVal = true;
            break;
        default:
            std::cout << "Invalid data value detected!" << std::endl;
            break;
        }

        // Check if dictionary has definition of row id
        int entry = users.Has(nextId);
        if (entry == -1) {
            // New entry
            users.AddEntry(nextId, &trueVal);
        } else {
            // Old entry
            if (users.GetLastValue(&entry)) {
                // Last entry was connection
                if (trueVal) {
                    // New value is connection
                    std::cout << "Disorganised data detected! (expected disconnection) " << *nextId << std::endl;
                    (*data)[i].id = 0;
                } else {
                    // New value is disconnection CORRECT
                    users.SetValue(&entry, &trueVal);
                }
            } else {
                // Last entry was disconnection
                if (trueVal) {
                    // New value is connection CORRECT
                    users.SetValue(&entry, &trueVal);
                } else {
                    // New value is disconnection
                    std::cout << "Disorganised data detected! (expected connection) " << *nextId << std::endl;
                    (*data)[i].id = 0;
                }
            }
        }
    }
}