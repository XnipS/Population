#pragma once
#include <string>
#include <vector>

struct dataStructure {
    long time;
    long id;
    int value;
};

class csvLoader {
public:
    static std::vector<dataStructure> LoadCSV(std::string* filename);
    static void RepairData(std::vector<dataStructure>* data);

private:
    class dictionary {

    public:
        int Has(long* id)
        {
            for (int i = 0; i < ids.size(); i++) {
                if (*id == ids[i]) {
                    return i;
                }
            }
            return -1;
        };
        bool GetLastValue(int* entry)
        {
            return lastValue[*entry];
        };
        void AddEntry(long* id, bool* value)
        {
            ids.push_back(*id);
            lastValue.push_back(*value);
        };
        void SetValue(int* entry, bool* value)
        {
            lastValue[*entry] = *value;
        };

    private:
        std::vector<long> ids;
        std::vector<bool> lastValue;
    };
};