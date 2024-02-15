#pragma once

#include "csvLoader.h"
#include <cstddef>
#include <string>
#include <vector>

class renderEngine {
public:
    renderEngine();
    ~renderEngine();

    char* path;
    finalData fData;

    void Initialise(const char* title, int w, int h);

    void Update();
    void Render();
    void Clean();
    bool Running() { return isRunning; };

private:
    // Render Engine
    std::string popup_id = "";
    int tick = 0;
    bool isRunning;
};