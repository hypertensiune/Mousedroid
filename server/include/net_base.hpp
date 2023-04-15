#pragma once

#ifdef _WIN32
    #define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#ifdef _WIN32
    #include "input/WindowsInputManager.hpp"
    typedef InputManager::Windows INPUT_MANAGER;
#endif
#ifdef __unix__
    #include "input/LinuxInputManager.hpp"
    typedef InputManager::Linux INPUT_MANAGER;
#endif

typedef asio::ip::tcp::socket ASIO_SOCK;