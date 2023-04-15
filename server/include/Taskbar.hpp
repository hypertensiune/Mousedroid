#pragma once

#include <wx/wx.h>
#include <wx/taskbar.h>

class wxMTaskbar: public wxTaskBarIcon
{
    public:
        wxMTaskbar(wxFrame *_frame);

    private:
        wxFrame *frame = nullptr;
        
        wxDECLARE_EVENT_TABLE();

        void OnDoubleClick(wxTaskBarIconEvent &evt);
        void Delete();
};