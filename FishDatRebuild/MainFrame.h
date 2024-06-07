#pragma once
#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

    struct Catch {
        int count = 0;
        double weight = 0.0;
    };

    struct Revir {
        int visits = 0;
        std::map<std::string, Catch> catches;
    };

    void processFile(const wxString& fileName, const wxString& outputFile, std::map<std::string, Revir>& revirs, int& totalVisits, std::map<std::string, Catch>& totalCatches);
    void createHtml(const wxString& outputFile, const std::map<std::string, Revir>& revirs, int totalVisits, const std::map<std::string, Catch>& totalCatches);
    void otvorClicked(wxCommandEvent& evt);

private:
    void OnPrehladClicked(wxCommandEvent& evt);
    void umiestnenieClicked(wxCommandEvent& evt);
    void spracujClicked(wxCommandEvent& evt);
    
    wxDECLARE_EVENT_TABLE();
};
