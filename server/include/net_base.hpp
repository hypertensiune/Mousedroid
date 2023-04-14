#ifndef NET_BASE1_H
#define NET_BASE1_H

#ifdef _WIN32
    #define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "InputManager.hpp"

typedef asio::ip::tcp::socket ASIO_SOCK;

#endif