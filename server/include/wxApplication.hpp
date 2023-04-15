#pragma once

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

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