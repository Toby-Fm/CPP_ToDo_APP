#include "header/App.h"
#include "header/main.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainFrame = new MainFrame(wxT("To-Do List"));
	mainFrame->SetClientSize(800, 600);
	mainFrame->SetMinSize(wxSize(300, 600));
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
