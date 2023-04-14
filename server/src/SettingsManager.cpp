#include "SettingsManager.hpp"

SettingsManager::SettingsManager()
{
    char buff[128] = { 0 };
    DWORD buffSize = sizeof(buff);

    LONG s2 = RegGetValueW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Mousedroid", RRF_RT_REG_SZ, nullptr, &buff, &buffSize);
    
    settings["RUN_STARTUP"] = std::to_string((s2 == ERROR_SUCCESS));

    LoadConfigs();
}

void SettingsManager::LoadConfigs()
{
    std::ifstream fin("config.ini");

    std::string line;
    while(std::getline(fin, line))
    {
        size_t pos = line.find('=');
        std::string setting = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        settings[setting] = value; 
    }

    fin.close();

    InputManager::MOVE_SENSITIVITY = std::atoi(settings["MOVE_SENSITIVITY"].c_str());
    InputManager::SCROLL_SENSITIVITY = std::atoi(settings["SCROLL_SENSITIVITY"].c_str());
}

void SettingsManager::SaveConfigs()
{
    std::ofstream fout("config.ini");
    for(auto &pair : settings)
    {
        fout << pair.first << "=" << pair.second << "\n";
    }
}

void SettingsManager::SetRunAtStartup(bool enabled)
{
    if(enabled)
    {
        std::string progPath = GetFilePath();
        LONG status = RegSetKeyValueA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "Mousedroid", REG_SZ, progPath.c_str(), (progPath.size() + 1) * sizeof(char));
    }
    else
        LONG status = RegDeleteKeyValueW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"Mousedroid");
}

void SettingsManager::SetMinimizeToTaskbar(bool enabled)
{
    settings["MINIMIZE_TASKBAR"] = std::to_string(enabled);
    SaveConfigs();
}

void SettingsManager::SetMoveSensitivity(int value)
{
    settings["MOVE_SENSITIVITY"] = std::to_string(value);
    InputManager::MOVE_SENSITIVITY = value;
    SaveConfigs();
}

void SettingsManager::SetScrollSensitivity(int value)
{
    settings["SCROLL_SENSITIVITY"] = std::to_string(value);
    InputManager::SCROLL_SENSITIVITY = value;
    SaveConfigs();
}

std::string SettingsManager::GetFilePath()
{
    #ifdef _WIN32
        char buffer[128] = {0};
        GetModuleFileNameA(NULL, buffer, 128);
        std::cout << buffer << "\n";
        return std::string(buffer);
    #endif

    return "abc";
}

bool SettingsManager::GetRunAtStartup()
{
    return settings["RUN_STARTUP"] == "1";
}

bool SettingsManager::GetMinToTaskbar()
{
    return settings["MINIMIZE_TASKBAR"] == "1";
}

int SettingsManager::GetMoveSensitivity()
{
    return std::atoi(settings["MOVE_SENSITIVITY"].c_str());
}

int SettingsManager::GetScrollSensitivity()
{
    return std::atoi(settings["SCROLL_SENSITIVITY"].c_str());
}