#pragma once

#include <wx/wx.h>

#include "wxMain.hpp"
#include "Server.hpp"
#include "SettingsManager.hpp"
#include "Logger.hpp"

class wxApplication : public wxApp
{
    public:
        wxApplication();
        ~wxApplication();

        virtual bool OnInit() override;
        virtual int OnExit() override;
        
    private:
        wxMain *main_frame = nullptr;
        Server *server = nullptr;
        
        SettingsManager settings;
};