#pragma once

#include <string>

extern int CONNECTION_USB;
extern int CONNECTION_WIFI;

struct Device
{
    int ID;
    std::string Address;
    std::string Manufacturer;
    std::string Name;
    std::string Model;
    int ctype;
};