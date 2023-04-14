#include "wxApplication.hpp"

wxIMPLEMENT_APP(wxApplication);

wxApplication::wxApplication() { }
wxApplication::~wxApplication() { }

bool wxApplication::OnInit()
{    
    wxInitAllImageHandlers();
    
    wxIcon icon;
    icon.LoadFile("app.ico", wxBITMAP_TYPE_ICO);

    main_frame = new wxMain(settings, icon);

    server = new Server();
    std::pair<std::string, std::string> hostInfo = server->getHostInfo();
    
    main_frame->SetHostInfo(hostInfo.first, hostInfo.second);

    if(!settings.GetRunAtStartup())
        main_frame->Show();
    
    server->setDeviceList(main_frame->wxdevlist);
    server->start();
    
    return true;
}

int wxApplication::OnExit()
{
    server->stop();

    delete server;

    Logger::monitor->Destroy();

    return wxApp::OnExit();
}
