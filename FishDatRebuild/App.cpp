#include "App.h"
#include "MainFrame.h"
#include<wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainframe = new MainFrame("FishDat");
	mainframe->SetClientSize(550, 250);
	mainframe->Center();
	mainframe->Show();
	wxInitAllImageHandlers();
	return true;
}