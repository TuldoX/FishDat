#include <wx/wx.h>
#include "MainFrame.h"
#include <stdexcept>

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

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(PREHLAD_ID, MainFrame::OnPrehladClicked)
EVT_BUTTON(SPRACUJ_ID, MainFrame::spracujClicked)
EVT_BUTTON(UMIESTNENIE_ID, MainFrame::umiestnenieClicked)
EVT_BUTTON(OTVOR_ID, MainFrame::otvorClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
    panel->SetFont(panel->GetFont().Scale(1.15));
    this->SetFocus();
    panel->SetBackgroundColour(wxColour(137, 207, 243));

    wxIcon icon;
    icon.LoadFile("logo.ico", wxBITMAP_TYPE_ICO);
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

void MainFrame::OnPrehladClicked(wxCommandEvent& evt) {
    wxFileDialog openFileDialog(this, _("Select file"), "", "", "CSV files (*.csv)|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    fileName = openFileDialog.GetPath();

    wxStaticText* cestaKSuboru = wxStaticCast(FindWindowById(CESTA_ID), wxStaticText);
    cestaKSuboru->SetToolTip(fileName);

    wxString truncFile = fileName.length() > 51 ? fileName.substr(0, 46) + "..." : fileName;
    cestaKSuboru->SetLabel(truncFile);
}

void MainFrame::umiestnenieClicked(wxCommandEvent& evt) {
    wxDirDialog dirDialog(this, _("Select directory"), "", wxDD_DIR_MUST_EXIST);

    if (dirDialog.ShowModal() == wxID_CANCEL) return;

    outputFile = dirDialog.GetPath();

    wxStaticText* cestaKAdresaru = wxStaticCast(FindWindowById(VYSTUP_SUBOR_ID), wxStaticText);
    cestaKAdresaru->SetToolTip(outputFile);

    wxString truncatedPath = outputFile.length() > 51 ? outputFile.substr(0, 46) + "..." : outputFile;
    cestaKAdresaru->SetLabel(truncatedPath);
}

void MainFrame::spracujClicked(wxCommandEvent& evt) {
    
    if (fileName == "") {
        wxMessageBox("File not selected!", "Too few arguments", wxOK | wxICON_ERROR);
        return;
    }
    else if (outputFile == "") {
        wxMessageBox("Directory not selected!", "Too few arguments", wxOK | wxICON_ERROR);
        return;
    }
    
    std::map<std::string, Revir> revirs;
    int totalVisits = 0;
    std::map<std::string, Catch> totalCatches;

    processFile(fileName, outputFile, revirs, totalVisits, totalCatches);
}

void MainFrame::processFile(const wxString& fileName, const wxString& outputFile, std::map<std::string, Revir>& revirs, int& totalVisits, std::map<std::string, Catch>& totalCatches) {
    std::ifstream fin(fileName.ToStdString());
    if (!fin) {
        wxMessageBox("Could not open the file: " + fileName, "File Open Error", wxOK | wxICON_ERROR);
        return;
    }

    // Skip the first line
    std::string header;
    std::getline(fin, header);

    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream lineStream(line);
        std::string revir, species, weightStr;
        std::getline(lineStream, revir, ';');
        std::getline(lineStream, species, ';');
        std::getline(lineStream, weightStr, ';');

        if (species != "-" && weightStr != "-" && !weightStr.empty() && std::all_of(weightStr.begin(), weightStr.end(), [](char c) { return std::isdigit(c) || c == '.'; })) {
            double weight = std::stod(weightStr);

            revirs[revir].visits++;
            revirs[revir].catches[species].count++;
            revirs[revir].catches[species].weight += weight;

            totalVisits++;
            totalCatches[species].count++;
            totalCatches[species].weight += weight;
        }
        else {
            revirs[revir].visits++;
            totalVisits++;
        }
    }

    fin.close();
    createHtml(outputFile, revirs, totalVisits, totalCatches);
    wxMessageBox("The file was processed correctly", "File Processed", wxOK | wxICON_INFORMATION);
}

void MainFrame::createHtml(const wxString& outputFile, const std::map<std::string, Revir>& revirs, int totalVisits, const std::map<std::string, Catch>& totalCatches) {
    std::string outputfile = outputFile.ToStdString() + "/rocny_sumar.html";
    std::ofstream fout(outputfile);
    if (!fout) {
        wxMessageBox("Could not open the file: " + outputFile, "File Open Error", wxOK | wxICON_ERROR);
        return;
    }

    fout << "<!DOCTYPE html>\n<html lang=\"sk\">\n<head>\n";
    fout << "<title>Ro&#269;ný sumár</title>\n";
    fout << "<style>\n";
    fout << "body { font-family: Arial, sans-serif; }\n";
    fout << "h1, h2 { text-align: center; }\n";
    fout << "table { width: 80%; margin: 0 auto; border-collapse: collapse; }\n";
    fout << "table, th, td { border: 1px solid black; }\n";
    fout << "th, td { padding: 8px; text-align: left; }\n";
    fout << "th { background-color: #f2f2f2; }\n";
    fout << "tr:nth-child(even) { background-color: #f9f9f9; }\n";
    fout << "</style>\n</head>\n<body>\n";
    fout << "<h1>Celoro&#269;ný sumár</h1>\n";

    fout << "<h2>Rev&iacute;ry</h2>\n";
    for (const auto& revirPair : revirs) {
        const std::string& revir = revirPair.first;
        const Revir& revirData = revirPair.second;
        fout << "<table>\n";
        fout << "<tr><th colspan='3'>Revír: " << revir << "</th></tr>\n";
        fout << "<tr><td colspan='3'>Po&#269;et vych&aacute;dzok: " << revirData.visits << "</td></tr>\n";
        fout << "<tr><th>Druh</th><th>Ks</th><th>Hmotnos&#357;(kg)</th></tr>\n";
        if (revirData.catches.empty()) {
            fout << "<tr><td>" << "-" << "</td><td>" << "-" << "</td><td>" << "-" << " </td></tr>\n";
        }
        else {
            for (const auto& catchPair : revirData.catches) {
                const std::string& species = catchPair.first;
                const Catch& catchData = catchPair.second;
                fout << "<tr><td>" << species << "</td><td>" << catchData.count << "</td><td>" << catchData.weight << "</td></tr>\n";
            }
        }
        fout << "</table>\n<br>\n";
    }

    fout << "<h2>Celkom</h2>\n";
    fout << "<table>\n";
    fout << "<tr><td colspan='3'>Po&#269;et vych&aacute;dzok: " << totalVisits << "</td></tr>\n";
    fout << "<tr><th>Druh</th><th>Ks</th><th>Hmotnos&#357;(kg)</th></tr>\n";
    for (const auto& catchPair : totalCatches) {
        const std::string& species = catchPair.first;
        const Catch& catchData = catchPair.second;
        fout << "<tr><td>" << species << "</td><td>" << catchData.count << "</td><td>" << catchData.weight << "</td></tr>\n";
    }
    fout << "</table>\n</body>\n</html>\n";

    fout.close();
}

void MainFrame::otvorClicked(wxCommandEvent& evt){
    wxString outputHtmlFile = outputFile + "\\rocny_sumar.html";
    if (!wxFileExists(outputHtmlFile)) {
        wxMessageBox("Output file does not exist: " + outputHtmlFile, "File Not Found", wxOK | wxICON_ERROR);
        return;
    }
    wxLaunchDefaultBrowser(outputHtmlFile);
}