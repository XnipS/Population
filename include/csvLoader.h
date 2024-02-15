#pragma once
#include <string>
#include <vector>

struct dataStructure {
    long time;
    long id;
    int value;
};

struct finalData {
    std::vector<long> ids;
    std::vector<long> totalTimes;
    std::vector<long> lastTime;
    void AddEntry(long* id, long total, long* last)
    {
        ids.push_back(*id);
        totalTimes.push_back(total);
        lastTime.push_back(*last);
    };
    int Has(long* id)
    {
        for (int i = 0; i < ids.size(); i++) {
            if (*id == ids[i]) {
                return i;
            }
        }
        return -1;
    };
    int HasTime(long* time)
    {
        for (int i = 0; i < totalTimes.size(); i++) {
            if (*time == totalTimes[i]) {
                return i;
            }
        }
        return -1;
    };
};

class csvLoader {
public:
    static std::vector<dataStructure> LoadCSV(char* filename);
    static void RepairRawData(std::vector<dataStructure>* data);
    static finalData SumRawData(std::vector<dataStructure>* data);
    static void SortFinalData(finalData* data);

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