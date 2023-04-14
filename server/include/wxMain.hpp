#pragma once

#define APP_WIDTH 500
#define APP_HEIGHT 450

#define APP_MARGIN 10

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/msgdlg.h>
#include <wx/hyperlink.h>

#include "wxDeviceList.hpp"
#include "SettingsManager.hpp"
#include "Taskbar.hpp"
#include "Logger.hpp"

#define CHK_STARTUP 1001
#define CHK_MINTASK 1002
#define SL_MOVE 1003
#define SL_SCROLL 1004

#define HOST_NAME_TXT 1010
#define HOST_IP_TXT 1011

class wxMain: public wxFrame
{
    public:
        wxDeviceList *wxdevlist = nullptr;
        wxPanel *tab_log = nullptr;

        wxMain(SettingsManager &_settings, wxIcon &icon);
        ~wxMain();
    
        void SetHostInfo(std::string _Hostname, std::string _IpAddress);
        
    private:
        wxPanel *tab_status = nullptr;
        wxPanel *tab_settings = nullptr;
        wxNotebook *wxnotebook = nullptr;
        wxMTaskbar *taskbar = nullptr;

        SettingsManager &settings;
        
        wxDECLARE_EVENT_TABLE();    

        void InitStatusTab();   
        void InitLogTab();
        void InitSettingsTab();

        void CheckboxCmdHandler(wxCommandEvent &evt);
        void SliderCmdHandler(wxScrollEvent &evt);

        void HideWindow(wxIconizeEvent &evt);
        void CloseWindow(wxCloseEvent &evt);
};
