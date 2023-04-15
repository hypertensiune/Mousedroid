#include "Client.hpp"

Client::Client(int _ID, asio::io_context &_context, ASIO_SOCK _socket, INPUT_MANAGER &_im, std::function<void(std::shared_ptr<Client>)> _onClientDisconnect) : 
    ID(_ID),
    context(_context), 
    socket(std::move(_socket)),
    im(_im),
    OnClientDisconnect(_onClientDisconnect)
{
    IP_ADDRESS = socket.remote_endpoint().address().to_string();
    buff = std::vector<char>(128);
}

void Client::Start()
{
    size_t bytes = socket.read_some(asio::buffer(buff.data(), buff.size()));

    std::istringstream f(std::string(buff.data(), bytes).substr(2));
    std::string tok;
    while(std::getline(f, tok, '_'))
    {
        deviceInfo.push_back(tok);
    }

    Read();
}

void Client::Read()
{
    socket.async_read_some(asio::buffer(buff.data(), buff.size()), [this](std::error_code ec, size_t bytes){
        if(!ec)
        {
            im.execute(std::string(buff.data(), bytes));
            Read();
        }
        else
        {
            LOG("[SERVER] DEVICE DISCONNECTED", IP_ADDRESS);
            Close();
        }
    });
}

void Client::Close(bool serverStopped)
{
    socket.close();
    
    if(!serverStopped)
        OnClientDisconnect(this->shared_from_this());
}