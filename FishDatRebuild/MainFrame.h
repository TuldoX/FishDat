#pragma once
#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm> // for std::find_if
#include <map>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

    struct Revir {
        std::string cislo;
        int pocetVychadzok;
    };

    struct Ryba {
        std::string cisloReviru;
        std::string druh;
        float hmotnost;
    };

    void processFile(wxString& fileName,wxString& outputFile, std::vector<Ryba>& ulovky, std::vector<Revir>& reviry) {
        std::string filename = fileName.ToStdString();

        if (filename.empty()) {
            wxMessageBox("File not selected", "File Open Error", wxOK | wxICON_ERROR);
            return;
        }

        std::ifstream fin;
        try {
            fin.open(filename);
            if (!fin) {
                wxMessageBox("Could not open the file: " + fileName, "File Open Error", wxOK | wxICON_ERROR);
                return;
            }

            std::string firstLine;
            std::getline(fin, firstLine);

            std::string line;
            while (std::getline(fin, line)) {
                std::istringstream lineStream(line);

                std::string revirNum, fish, dlzka, weight;
                std::getline(lineStream, revirNum, ';');
                std::getline(lineStream, fish, ';');
                std::getline(lineStream, dlzka, ';');
                std::getline(lineStream, weight, ';');

                if (!revirNum.empty() && !fish.empty() && !dlzka.empty() && !weight.empty()) {

                    if (fish != "-") {
                        Ryba ryba;
                        ryba.cisloReviru = revirNum;
                        ryba.druh = fish;
                        ryba.hmotnost = std::stof(weight);
                        ulovky.push_back(ryba);
                    }
                    
                    auto it = std::find_if(reviry.begin(), reviry.end(), [&](const Revir& r) { return r.cislo == revirNum; });
                    if (it != reviry.end()) {
                        it->pocetVychadzok++;
                    }
                    else {
                        Revir newRevir;
                        newRevir.cislo = revirNum;
                        newRevir.pocetVychadzok = 1;
                        reviry.push_back(newRevir);
                    }
                }
            }

            fin.close();
            wxMessageBox("The file was processed correctly", "File Processed", wxOK | wxICON_INFORMATION);
        }
        catch (const std::exception& e) {
            wxMessageBox("An error occurred while processing the file: " + wxString(e.what()), "Processing Error", wxOK | wxICON_ERROR);
            if (fin.is_open()) {
                fin.close();
            }
        }
        createHtml(outputFile, ulovky, reviry);

    }
    void createHtml(wxString& outputFile, std::vector<Ryba>& ulovky, std::vector<Revir>& reviry) {

        std::string outputfile = outputFile.ToStdString() + "\\rocny_sumar.html";
        std::ofstream fout(outputfile);
        if (!fout) {
            wxMessageBox("Could not open the file: " + outputFile, "File Open Error", wxOK | wxICON_ERROR);
            return;
        }

        fout << "<!DOCTYPE html>";
        fout << "<html>\n";
        fout << "<head>\n";
        fout << "<title>Roèný sumár</title>\n";
        fout << "<style>\n";
        fout << "body { font-family: Arial, sans-serif; }";
        fout << "h1, h2 { text-align: center; }\n";
        fout << "table { width: 80%; margin: 0 auto; border-collapse: collapse; }\n";
        fout << "table, th, td { border: 1px solid black; }\n";
        fout << "th, td { padding: 8px; text-align: left; }\n";
        fout << "th { background-color: #f2f2f2; }\n";
        fout << "tr:nth-child(even) { background-color: #f9f9f9; }\n";
        fout << "</style>\n</head>\n<body>\n";
        fout << "<h1>Celoroèný sumár</h1>\n";
        
    }
    

    

private:
    void OnPrehladClicked(wxCommandEvent& evt);
    void umiestnenieClicked(wxCommandEvent& evt);
    void spracujClicked(wxCommandEvent& evt);
    wxDECLARE_EVENT_TABLE();
};
