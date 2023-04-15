#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "input/InputManager.hpp"
#include "Logger.hpp"

#ifdef _WIN32
    #include "windows.h"
#endif

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

        void ADBOn();
        void ADBOff();
        bool ADBOnOff();
        bool ADBState();

    private:
        std::ifstream fin;
        std::map<std::string, std::string> settings;

        bool bRunAtStartup;
        bool bMinToTaskbar;

        bool bAdbStarted;

        std::string GetFilePath();
        void LoadConfigs();
        void SaveConfigs();
};