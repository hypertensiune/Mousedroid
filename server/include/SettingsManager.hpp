#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "InputManager.hpp"

class SettingsManager
{
    public:
        SettingsManager();

        void SetRunAtStartup(bool enabled);
        void SetMinimizeToTaskbar(bool enabled);
        bool GetRunAtStartup();
        bool GetMinToTaskbar();
        void SetMoveSensitivity(int value);
        void SetScrollSensitivity(int value);
        int GetMoveSensitivity();
        int GetScrollSensitivity();

    private:
        std::ifstream fin;
        std::map<std::string, std::string> settings;

        bool bRunAtStartup;
        bool bMinToTaskbar;

        std::string GetFilePath();
        void LoadConfigs();
        void SaveConfigs();
};