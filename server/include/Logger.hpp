#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <time.h>

#define LOG(args...) Logger::log(args)

namespace Logger
{
    extern int rows;
    extern wxListView *monitor;

    extern std::stringstream output;

    void displayOutput();
    
    template <typename T>
    void log(T t)
    {
        output << t;
        displayOutput();
    }

    template <typename T, typename... Args>
    void log(T t, Args... args)
    {
        output << t;
        log(args...);
    }
}