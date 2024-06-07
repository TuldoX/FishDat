#include <wx/wx.h>
#include "MainFrame.h"

wxString fileName;
wxString outputFile;

enum IDs {
    PREHLAD_ID = 2,
    CESTA_ID = 3,
    SPRACUJ_ID = 4,
    UMIESTNENIE_ID = 5,
    OTVOR_ID = 6,
    VYSTUP_SUBOR_ID = 7
};

//EVENT HANDLERY NA FUNKCIE
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(PREHLAD_ID, MainFrame::OnPrehladClicked)
EVT_BUTTON(SPRACUJ_ID, MainFrame::spracujClicked)
EVT_BUTTON(UMIESTNENIE_ID, MainFrame::umiestnenieClicked)
wxEND_EVENT_TABLE()

//Rozlozenie HOTOVO
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {


    wxPanel* panel = new wxPanel(this);
    panel->SetFont(panel->GetFont().Scale(1.15));
    this->SetFocus();
    panel->SetBackgroundColour(wxColour(137, 207, 243));

    wxIcon icon;
    icon.LoadFile("C:/Users/NTB/Dokumenty/Skola/Projekty/FishDatRebuild/FishDatRebuild/logo.ico", wxBITMAP_TYPE_ICO);
    this->SetIcon(icon);


    wxStaticText* cestaKSuboru = new wxStaticText(panel, CESTA_ID, "", wxPoint(25, 35), wxSize(322.5, -1), wxBORDER_SIMPLE);
    cestaKSuboru->SetBackgroundColour(wxColour(205, 245, 253));

    wxButton* prehladavanie = new wxButton(panel, PREHLAD_ID, "Select file", wxPoint(390, 34), wxSize(140, -1));

    wxStaticText* cestaVystupSuboru = new wxStaticText(panel, VYSTUP_SUBOR_ID, "", wxPoint(25, 100), wxSize(322.5, -1), wxBORDER_SIMPLE);
    cestaVystupSuboru->SetBackgroundColour(wxColour(205, 245, 253));

    wxButton* umiestnenie = new wxButton(panel, UMIESTNENIE_ID, "Select directory", wxPoint(390, 99), wxSize(140, -1));

    wxButton* spracovanie = new wxButton(panel, SPRACUJ_ID, "Process file", wxPoint(205, 155), wxSize(160, -1));

    wxButton* otvorenie = new wxButton(panel, OTVOR_ID, "Open output", wxPoint(205, 200), wxSize(160, -1));
}

//Vybranie suboru HOTOVO OUTPUT  fileName
void MainFrame::OnPrehladClicked(wxCommandEvent& evt) {

    wxFileDialog openFileDialog(this, _("Select file"), "", "", "CSV files (*.csv)|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    fileName = openFileDialog.GetPath();

    wxStaticText* cestaKSuboru = wxStaticCast(FindWindowById(CESTA_ID), wxStaticText);
    cestaKSuboru->SetToolTip(fileName);

    wxString truncFile;
    if (fileName.length() > 51)
        truncFile = fileName.substr(0, 46) + "...";
    else
        truncFile = fileName;

    cestaKSuboru->SetLabel(truncFile);
}

//Vybranie umiestnenia HOTOVO OUTPUT  outputFile
void MainFrame::umiestnenieClicked(wxCommandEvent& evt) {

    wxDirDialog dirDialog(this, _("Select directory"), "", wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() == wxID_CANCEL) return;

    outputFile = dirDialog.GetPath();

    wxStaticText* cestaKAdresaru = wxStaticCast(FindWindowById(VYSTUP_SUBOR_ID), wxStaticText);
    cestaKAdresaru->SetToolTip(outputFile);

    wxString truncatedPath;
    if (outputFile.length() > 51)
        truncatedPath = outputFile.substr(0, 46) + "...";
    else
        truncatedPath = outputFile;

    cestaKAdresaru->SetLabel(truncatedPath);
}

void MainFrame::spracujClicked(wxCommandEvent& evt) {

    std::vector<Ryba> ryby;
    std::vector<Revir> revir;
    std::vector<Celkovo> celkovo;

    processFile(fileName, outputFile,ryby, revir,celkovo);
}