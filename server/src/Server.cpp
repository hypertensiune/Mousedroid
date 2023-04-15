#include "Server.hpp"

Server::Server(SettingsManager &_settings) : 
    acceptor(asio::ip::tcp::acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), PORT))),
    settings(_settings)
{
    settings.ADBOn();
}

Server::~Server() { }

void Server::setDeviceList(wxDeviceList *_wxdevlist)
{
    wxdevlist = _wxdevlist;
}

std::pair<std::string, std::string> Server::getHostInfo()
{
    std::string host_name = asio::ip::host_name();
        
    addrinfo *res;
    addrinfo hints;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    getaddrinfo(host_name.c_str(), NULL, &hints, &res);

    void *addr;
    char ipstr[INET_ADDRSTRLEN];

    while(res->ai_next != NULL)
        res = res->ai_next;

    addr = &((sockaddr_in *)res->ai_addr)->sin_addr;

    inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));

    return {host_name, ipstr};
}

void Server::start()
{
    try
    {
        do_accept();
        thread = std::thread([this](){ context.run(); });

        LOG("[SERVER] STARTED!");
    }
    catch(const std::exception& e)
    {
        LOG("[SERVER EXCEPTION]", *(e.what()));
    }
}

void Server::do_accept()
{
    acceptor.async_accept([&, this](asio::error_code ec, ASIO_SOCK socket){
        if (!ec)
        {
            LOG("[SERVER] DEVICE CONNECTED:", socket.remote_endpoint());
            
            std::shared_ptr<Client> conn = std::make_shared<Client>(currentID, context, std::move(socket), input_manager, std::bind(&Server::OnClientDisconnected, this, std::placeholders::_1));
            clients.push_back(std::move(conn));
            clients.back()->Start();

            OnClientConnected();
            currentID++;
        }
        else
        {
            LOG("[SERVER] ACCEPT CONNECTION ERROR", ec);
        }

        do_accept();
    });
}

void Server::stop()
{
    settings.ADBOff();

    for(auto c : clients)
    {
        c->Close(true);
    }
    
    context.stop();

    if(thread.joinable())
        thread.join();    
}

void Server::OnClientConnected()
{
    std::vector<std::string> v = clients.back()->deviceInfo;

    Device device = {currentID, clients.back()->IP_ADDRESS, v[0], v[1], v[2], static_cast<connection_type>(std::atoi(v[3].c_str()))};
    wxdevlist->AddDevice(device);
}

void Server::OnClientDisconnected(std::shared_ptr<Client> client)
{
    wxdevlist->RemoveDevice(client->ID);
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
}