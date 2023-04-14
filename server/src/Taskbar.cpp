#include "Taskbar.hpp"

wxBEGIN_EVENT_TABLE(wxMTaskbar, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK(wxMTaskbar::OnDoubleClick)
wxEND_EVENT_TABLE()

wxMTaskbar::wxMTaskbar(wxFrame *_frame) : frame(_frame) { }

void wxMTaskbar::OnDoubleClick(wxTaskBarIconEvent &evt)
{
    frame->Iconize(false);
    frame->SetFocus();
    frame->Raise();
    frame->Show();

    evt.Skip();
}

void wxMTaskbar::Delete()
{
    delete frame;
}