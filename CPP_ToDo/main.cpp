#include "main.h"
#include <vector>
#include <string>
#include "Task.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindEventHandlers();
}

void MainFrame::CreateControls()
{
	wxFont healineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(healineFont);

	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	checklistBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
}

void MainFrame::BindEventHandlers()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checklistBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& event)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode())
	{
		case WXK_DELETE: 
		{
			DeleteSelectedTasks();
			break;
		}
		case WXK_UP: 
		{
			MoveSelectedTasks(-1);
			break;
		}
		case WXK_DOWN: 
		{
			MoveSelectedTasks(1);
			break;
		}
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checklistBox->IsEmpty())
	{
		return;
	}

	wxMessageDialog dialog(this, "Möchtest du wirklich alle Tasks löschen?", "Löschen", wxYES_NO | wxCANCEL | wxICON_QUESTION);
}

void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();
	if (!description.IsEmpty())
	{
		checklistBox->Insert(description, checklistBox->GetCount());
		inputField->Clear();
	}

	inputField->SetFocus();
}

void MainFrame::DeleteSelectedTasks()
{
	int selectedIndex = checklistBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}
	checklistBox->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTasks(int offset)
{
	int selectedIndex = checklistBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}

	int newIndex = selectedIndex + offset;
	if (newIndex >= 0 && newIndex < checklistBox->GetCount())
	{
		SwapTasks(selectedIndex, newIndex);
		checklistBox->SetSelection(newIndex, true);
	}
}

void MainFrame::SwapTasks(int i, int j)
{
	Task taskI{ checklistBox->GetString(i).ToStdString(), checklistBox->IsChecked(i)};
	Task taskJ{ checklistBox->GetString(j).ToStdString(), checklistBox->IsChecked(j)};

	checklistBox->SetString(i, taskJ.description);
	checklistBox->Check(i, taskJ.done);

	checklistBox->SetString(j, taskI.description);
	checklistBox->Check(j, taskI.done);
}
