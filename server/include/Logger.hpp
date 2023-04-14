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

/*class Logger
{
    public:
        static void Init(wxWindow *parent)
        {
            monitor = new wxListView(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
            monitor->EnableSystemTheme(false);

            monitor->AppendColumn("Time");
            monitor->AppendColumn("Message", wxLIST_FORMAT_LEFT, 380);

            rows = 0;
        }

        template<typename T>
        static void log(T t)
        {
            InsertIntoMonitor(t);
        }

        template<typename T, typename... Args>
        static void log(T t, Args... args)
        {
            InsertIntoMonitor(t);
            log(args...);
        }

        static wxListView* getWxListView()
        {
            return monitor;
        }

    private:
        static int rows;
        static wxListView *monitor;

        template<typename T>
        static void InsertIntoMonitor(T in)
        {
            time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::string time = std::string(ctime(&now) + 11, 8);

            monitor->InsertItem(rows, time);
            monitor->SetItem(rows++, 1, in);
        }
};*/
