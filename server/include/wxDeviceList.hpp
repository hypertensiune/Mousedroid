#pragma once

#include <wx/listctrl.h>
#include <wx/systhemectrl.h>
#include <vector>

#include "Device.hpp"

class wxDeviceList: public wxListView
{
    public:
        wxDeviceList(wxWindow *parent, int id);
        ~wxDeviceList();

        void AddDevice(Device device);
        void RemoveDevice(int id);
    
    private:
        std::vector<Device> devices;

        void UpdateView(bool refresh = false);
        void InsertDevice(int i, Device &device);
};