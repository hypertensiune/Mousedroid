#pragma once

#include "net_base.hpp"

#include <vector>
#include <functional>
#include <sstream>

#include "Logger.hpp"

class Client: public std::enable_shared_from_this<Client>
{
    public:
        int ID;
        std::string IP_ADDRESS;
        std::vector<std::string> deviceInfo;
        
        Client(int _ID, asio::io_context &context, ASIO_SOCK _socket, INPUT_MANAGER &im, std::function<void(std::shared_ptr<Client>)> _onClientDisconnect);
        
        void Start();
        void Close(bool serverStopped = false);

    private:
        INPUT_MANAGER &im;
        ASIO_SOCK socket;
        asio::io_context &context;
        std::vector<char> buff;

        std::function<void(std::shared_ptr<Client>)> OnClientDisconnect;

        void Read();
};