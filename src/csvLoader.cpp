#include "../include/csvLoader.h"
#include "../include/core.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<dataStructure> csvLoader::LoadCSV(char* filename)
{
    // Variables
    std::ifstream target(filename);
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
    return dataFile;
}

void csvLoader::RepairRawData(std::vector<dataStructure>* data)
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
                    std::cout << "Disorganised data detected! (expected disconnection) ID:" << *nextId << std::endl;
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
                    std::cout << "Disorganised data detected! (expected connection) ID:" << *nextId << std::endl;
                    (*data)[i].id = 0;
                }
            }
        }
    }
}

finalData csvLoader::SumRawData(std::vector<dataStructure>* data)
{
    finalData ret;
    long currentId;
    // Foreach data point
    for (int i = 0; i < data->size(); i++) {
        currentId = (*data)[i].id;
        // Check if the id is valid
        if (currentId == 0) {
            continue;
        } else {
            int entry = ret.Has(&currentId);
            if (entry == -1) {
                // New entry
                ret.AddEntry(&currentId, 0, &(*data)[i].time);
            } else {
                // Old entry
                if ((*data)[i].value) {
                    // Connection
                    // Mark time
                    ret.lastTime[entry] = (*data)[i].time;
                } else {
                    // Disconnection
                    // Sum time since connection
                    long delta = (*data)[i].time - ret.lastTime[entry];
                    // Check for large connection time (due to csvLoader::RepairData)
                    if (delta > P_LARGETIME) {
                        std::cout << "Large time detected! ID:" << currentId << " DELTA:" << delta << " (UnixTime)" << std::endl;
                    }
                    ret.totalTimes[entry] += delta;
                }
            }
        }
    }
    return ret;
}

void csvLoader::SortFinalData(finalData* data)
{
    // Make new memory of data
    finalData newData = finalData(*data);
    // Sort data
    std::sort(std::begin(newData.totalTimes), std::end(newData.totalTimes));
    // Match ids to new array
    for (int i = 0; i < newData.totalTimes.size(); i++) {
        int entry = data->HasTime(&newData.totalTimes[i]);
        if (entry == -1) {
            // New entry????
            std::cout << "New array is missing data? (Memory error)" << std::endl;
        } else {
            // Assign true id
            newData.ids[i] = data->ids[entry];
        }
    }
    // Pass final data to pointer
    *data = finalData(newData);
}