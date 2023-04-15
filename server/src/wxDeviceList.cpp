#include "wxDeviceList.hpp"

wxDeviceList::wxDeviceList(wxWindow *parent, int id): wxListView(parent, id, wxDefaultPosition, wxDefaultSize, wxLC_REPORT)
{
    #ifdef _WIN32
        this->EnableSystemTheme(false);
    #endif

    this->AppendColumn("#", wxLIST_FORMAT_LEFT, 20);
    this->AppendColumn("Address", wxLIST_FORMAT_LEFT);
    this->AppendColumn("Status", wxLIST_FORMAT_LEFT, 45);
    this->AppendColumn("Name", wxLIST_FORMAT_LEFT);
    this->AppendColumn("Manufacturer", wxLIST_FORMAT_LEFT, 100);
    this->AppendColumn("Model", wxLIST_FORMAT_LEFT);
}

void wxDeviceList::InsertDevice(int i, Device &device)
{
    std::string conn = (device.ctype == USB) ? "USB" : "WIFI";

    this->InsertItem(i, std::to_string(i + 1));
    this->SetItem(i, 1, device.Address);
    this->SetItem(i, 2, conn);
    this->SetItem(i, 3, device.Name);
    this->SetItem(i, 4, device.Manufacturer);
    this->SetItem(i, 5, device.Model);
}

void wxDeviceList::UpdateView(bool refresh /*=false*/)
{
    if(refresh)
    {
        this->DeleteAllItems();
        for(int i = 0; i < devices.size(); i++)
        {
            InsertDevice(i, devices[i]);
        }
    }
    else
    {
        int i = this->GetItemCount();
        InsertDevice(i, devices[i]);
    }
}

void wxDeviceList::AddDevice(Device device)
{
    devices.push_back(device);
    UpdateView();
}

void wxDeviceList::RemoveDevice(int id)
{
    for(auto it = devices.begin(); it != devices.end(); it++)
    {
        if((*it).ID == id)
        {
            devices.erase(it);
            UpdateView(true);
            break;
        }
    }
}

wxDeviceList::~wxDeviceList()
{
}