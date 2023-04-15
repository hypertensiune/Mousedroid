#include "wxApplication.hpp"

wxIMPLEMENT_APP(wxApplication);

#include "icon.xpm"

wxApplication::wxApplication() { }
wxApplication::~wxApplication() { }

bool wxApplication::OnInit()
{    
    main_frame = new wxMain(settings);
    
    server = new Server(settings);

    std::pair<std::string, std::string> hostInfo = server->getHostInfo();
    
    main_frame->SetHostInfo(hostInfo.first, hostInfo.second);

    if(!settings.GetRunAtStartup())
        main_frame->Show();
    
    server->setDeviceList(main_frame->wxdevlist);
    server->start();

    main_frame->UpdateUI();

    return true;
}

int wxApplication::OnExit()
{
    server->stop();
    delete server;

    Logger::monitor->Destroy();

    return wxApp::OnExit();
}