#include "wxMain.hpp"

#include "icon.xpm"

wxBEGIN_EVENT_TABLE(wxMain, wxFrame)
    EVT_ICONIZE(wxMain::HideWindow)
    EVT_CLOSE(wxMain::CloseWindow)
    EVT_CHECKBOX(CHK_STARTUP, wxMain::CheckboxCmdHandler)
    EVT_CHECKBOX(CHK_MINTASK, wxMain::CheckboxCmdHandler)
    EVT_COMMAND_SCROLL(SL_MOVE, wxMain::SliderCmdHandler)
    EVT_COMMAND_SCROLL(SL_SCROLL, wxMain::SliderCmdHandler)
wxEND_EVENT_TABLE()

wxMain::wxMain(SettingsManager &_settings) : 
    wxFrame(NULL, wxID_ANY, "Mousedroid", wxDefaultPosition, wxSize(APP_WIDTH, APP_HEIGHT), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX),
    settings(_settings)
{
    wxnotebook = new wxNotebook(this, wxID_ANY, wxPoint(APP_MARGIN, APP_MARGIN), wxSize(APP_WIDTH - 23, APP_HEIGHT - 45), wxNB_TOP);

    tab_status = new wxPanel(wxnotebook, wxID_ANY);
    tab_log = new wxPanel(wxnotebook, wxID_ANY);
    tab_settings = new wxPanel(wxnotebook, wxID_ANY);

    wxnotebook->AddPage(tab_status, "Status", true);
    wxnotebook->AddPage(tab_settings, "Settings");
    wxnotebook->AddPage(tab_log, "Log");

    #ifdef _WIN32
        this->SetIcon(icon);
    #endif

    tb = new wxMTaskbar(this);
    tb->SetIcon(icon, "Mousedroid");
    
    InitStatusTab();
    InitSettingsTab();
    InitLogTab();
}   

void wxMain::InitStatusTab()
{    
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    // ==== This PC info ====
    wxStaticBoxSizer *ipsizer = new wxStaticBoxSizer(wxVERTICAL, tab_status, "This PC");

    wxBoxSizer *r1 = new wxBoxSizer(wxHORIZONTAL);

    r1->Add(new wxStaticText(tab_status, wxID_ANY, "Name: "));
    wxStaticText *sb1 = new wxStaticText(tab_status, HOST_NAME_TXT, "");
    sb1->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    r1->Add(sb1);

    wxBoxSizer *r2 = new wxBoxSizer(wxHORIZONTAL);
    r2->Add(new wxStaticText(tab_status, wxID_ANY, "IP: "));
    wxStaticText *sb2 = new wxStaticText(tab_status, HOST_IP_TXT, "");
    sb2->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    r2->Add(sb2);

    ipsizer->Add(r1);
    ipsizer->Add(r2);

    // ==== Device list ==== 
    wxdevlist = new wxDeviceList(tab_status, wxID_ANY);
        
    wxStaticBoxSizer *devsizer = new wxStaticBoxSizer(wxVERTICAL, tab_status, "Devices");
    devsizer->Add(wxdevlist, wxSizerFlags(1).Expand().Border(wxALL, 5));

    adbbutton = new wxButton(tab_status, wxID_ANY, "");
    adbbutton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &evt){
        bool res = settings.ADBOnOff();

        if(res)
            adbbutton->SetLabel("ADB On");
        else
            adbbutton->SetLabel("ADB Off");
    });

    devsizer->Add(adbbutton, wxSizerFlags().Right());


    topsizer->Add(ipsizer, wxSizerFlags().Expand().Border(wxALL, 5));
    topsizer->Add(devsizer, wxSizerFlags(1).Expand().Border(wxALL, 5));

    tab_status->SetSizerAndFit(topsizer);
}

void wxMain::InitLogTab()
{
    wxBoxSizer *topsizer3 = new wxBoxSizer(wxVERTICAL);
    
    wxListView *monitor = new wxListView(tab_log, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

    monitor->AppendColumn("Time");
    monitor->AppendColumn("Message", wxLIST_FORMAT_LEFT, 380);

    Logger::monitor = monitor;

    topsizer3->Add(monitor, wxSizerFlags(1).Expand());

    tab_log->SetSizerAndFit(topsizer3);
}

void wxMain::InitSettingsTab()
{
    wxBoxSizer *topsizer2 = new wxBoxSizer(wxVERTICAL);

    // ==== System settings ====
    wxStaticBoxSizer *sysboxsizer = new wxStaticBoxSizer(wxVERTICAL, tab_settings, "System");

    #ifdef _WIN32
        // Startup checkbox
        wxCheckBox *chk_startup = new wxCheckBox(tab_settings, CHK_STARTUP, "Run at startup");
        chk_startup->SetValue(settings.GetRunAtStartup());
        sysboxsizer->Add(chk_startup, wxSizerFlags().Border(wxTOP, 10));
    #endif

    // Minimize to taskbar checkbox
    wxCheckBox *chk_mintask = new wxCheckBox(tab_settings, CHK_MINTASK, "Minimize to taskbar");
    chk_mintask->SetValue(settings.GetMinToTaskbar());
    sysboxsizer->Add(chk_mintask, wxSizerFlags().Border(wxTOP | wxBOTTOM, 5));

    // ==== Controls settings ====
    wxStaticBoxSizer *ctrlboxsizer = new wxStaticBoxSizer(wxVERTICAL, tab_settings, "Controls");

    // Move sensitivity slider
    wxBoxSizer *b1 = new wxBoxSizer(wxHORIZONTAL);
    b1->Add(new wxStaticText(tab_settings, wxID_ANY, "Move sensitivity:"), wxSizerFlags().Border(wxTOP, 4));

    wxSlider *ms = new wxSlider(
        tab_settings, SL_MOVE, 10, 1, 20, wxDefaultPosition, wxSize(150, wxDefaultSize.y), 
        #ifdef _WIN32
            wxSL_TICKS 
        #else 
            wxSL_VALUE_LABEL
        #endif
    );

    ms->SetValue(settings.GetMoveSensitivity());
    b1->Add(ms);
    ctrlboxsizer->Add(b1);

    // Scroll sensitivity slider
    wxBoxSizer *b2 = new wxBoxSizer(wxHORIZONTAL);
    b2->Add(new wxStaticText(tab_settings, wxID_ANY, "Scroll sensitivity:"), wxSizerFlags().Border(wxTOP, 4));

    wxSlider *ss = new wxSlider(
        tab_settings, SL_SCROLL, 10, 1, 20, wxDefaultPosition, wxSize(150, wxDefaultSize.y), 
        #ifdef _WIN32
            wxSL_TICKS 
        #else 
            wxSL_VALUE_LABEL
        #endif
    );
    
    ss->SetValue(settings.GetScrollSensitivity());
    b2->Add(ss);
    ctrlboxsizer->Add(b2);

    // ==== Info ====
    wxBoxSizer *info = new wxBoxSizer(wxVERTICAL);

    wxStaticText *t = new wxStaticText(tab_settings, wxID_ANY, "Mousedroid v1.0");
    t->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    info->Add(t, wxSizerFlags().Center());
    info->Add(new wxHyperlinkCtrl(tab_settings, wxID_ANY, "Github", "https://github.com/hypertensiune/mousedroid"), wxSizerFlags().Center());


    topsizer2->Add(sysboxsizer, wxSizerFlags().Expand().Border(wxALL, 5));
    topsizer2->Add(ctrlboxsizer, wxSizerFlags().Expand().Border(wxALL, 5));
    topsizer2->Add(info, wxSizerFlags().Expand().Border(wxTOP, 150));

    tab_settings->SetSizerAndFit(topsizer2);
}

void wxMain::SetHostInfo(std::string _Hostname, std::string _IpAddress)
{
    ((wxStaticText*)FindWindowById(HOST_NAME_TXT))->SetLabelText(_Hostname);
    ((wxStaticText*)FindWindowById(HOST_IP_TXT))->SetLabelText(_IpAddress);
}

void wxMain::CloseWindow(wxCloseEvent &evt)
{
    wxMessageDialog *box = new wxMessageDialog(this, "This will disconnect all connected devices. Proceed?", "Confirm", wxYES_NO | wxICON_INFORMATION);

    int res = box->ShowModal();

    if(res == wxID_YES)
        evt.Skip();
}

void wxMain::HideWindow(wxIconizeEvent &evt)
{
    if(!settings.GetMinToTaskbar())
    {
        this->Hide();
        evt.Skip();
    }
}

void wxMain::CheckboxCmdHandler(wxCommandEvent &evt)
{
    if(evt.GetId() == CHK_STARTUP)
        settings.SetRunAtStartup(evt.IsChecked());
    else
        settings.SetMinimizeToTaskbar(evt.IsChecked());
}

void wxMain::SliderCmdHandler(wxScrollEvent &evt)
{
    if(evt.GetId() == SL_MOVE)
        settings.SetMoveSensitivity(evt.GetPosition());
    else
        settings.SetScrollSensitivity(evt.GetPosition());
}

void wxMain::UpdateUI()
{
    std::string label = (settings.ADBState()) ? "ADB On" : "ADB Off";
    adbbutton->SetLabel(label);
}

wxMain::~wxMain()
{
    delete tb;
}