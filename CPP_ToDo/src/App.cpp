#include "header/App.h"
#include "header/main.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainFrame = new MainFrame(wxT("To-Do List"));
	mainFrame->SetClientSize(800, 600);
	mainFrame->SetMinSize(wxSize(300, 600));
	mainFrame->Center();
	/*
	// Icon for the window png
	wxIcon icon;
	icon.LoadFile(wxT("../images/logo3.png", wxBITMAP_TYPE_PNG));
	mainFrame->SetIcon(icon);
	*/
	mainFrame->Show();
	return true;
}
