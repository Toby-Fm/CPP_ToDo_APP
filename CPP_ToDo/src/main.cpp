#include "header/main.h"
#include "header/Task.h"
#include <vector>
#include <string>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title), colorMode(false)
{
	CreateControls();
	BindEventHandlers();
	AddSavedTasks();
}

void MainFrame::CreateControls()
{
	wxFont healineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	//Objects
	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add");
	checklistBox = new wxCheckListBox(panel, wxID_ANY);
	clearButton = new wxButton(panel, wxID_ANY, "Clear");
	changeColorButton = new wxButton(panel, wxID_ANY, "Change Color");
	infoButton = new wxButton(panel, wxID_ANY, "Info");

	//Layout mit Sizern
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hSizer2 = new wxBoxSizer(wxHORIZONTAL);

	hSizer->Add(inputField, 5, wxEXPAND | wxRIGHT | wxBOTTOM, 5); // 5 Teile der verfügbaren Größe
	hSizer->Add(addButton, 1, wxEXPAND | wxLEFT | wxBOTTOM, 5); // 1 Teil der verfügbaren Größe
	
	vSizer->Add(headlineText, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);
	vSizer->Add(hSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	vSizer->Add(checklistBox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
	vSizer->Add(clearButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

	hSizer2->Add(changeColorButton, 1, wxEXPAND | wxLEFT | wxBOTTOM, 10);
	hSizer2->AddStretchSpacer(1);
	hSizer2->Add(infoButton, 1, wxEXPAND | wxRIGHT | wxBOTTOM, 10);
	
	vSizer->Add(hSizer2, 0, wxEXPAND);

	panel->SetSizer(vSizer);
	panel->Layout();
}

void MainFrame::BindEventHandlers()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checklistBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	checklistBox->Bind(wxEVT_CHECKLISTBOX, &MainFrame::OnCheckListBoxToggled, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
	changeColorButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangeColorButtonClicked, this);
	infoButton->Bind(wxEVT_BUTTON, &MainFrame::OnInfoButtonClicked, this);
}

void MainFrame::AddSavedTasks()
{
	std::vector<Task> tasks = LoadTasksFromFile("tasks.txt");
	for (const Task& task : tasks)
	{
		int index = checklistBox->GetCount();
		checklistBox->Insert(task.description, index);
		checklistBox->Check(index, task.done);
	}
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
		case WXK_F2:
		case 'R':
		case 'r':
		{
			int selectedIndex = checklistBox->GetSelection();
			if (selectedIndex != wxNOT_FOUND)
			{
				RenameTask(selectedIndex);
			}
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
	int result = dialog.ShowModal();

	if (result == wxID_YES)
	{
		checklistBox->Clear();
	}

}

void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	std::vector<Task> tasks;
	for (int i = 0; i < checklistBox->GetCount(); i++)
	{
		Task task;
		task.description = checklistBox->GetString(i);
		task.done = checklistBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTasksToFile(tasks, "tasks.txt");
	evt.Skip();
}

void MainFrame::OnInfoButtonClicked(wxCommandEvent& evt)
{
	wxMessageBox(L"\n\nTippe was ein + 'Enter' und du Fügst was Hinzu.\n\n Wähle eine Task aus und drück 'UP' or 'DOWN' um die Task zu verschieben.\n\n Wähle eine Task aus und drücke 'F2' oder 'R' um die Beschreibung zu ändern.\n\n Wähle eine Task aus und drücke 'ENTF' um eine Task zu löschen.", "Info", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnChangeColorButtonClicked(wxCommandEvent& evt)
{
	if (colorMode) { //Heller modus
		// Farben für den ersten Modus
		panel->SetBackgroundColour(wxColour(255, 255, 255));
		addButton->SetBackgroundColour(wxColour(255, 255, 255));
		addButton->SetForegroundColour(wxColour(0, 0, 0));
		inputField->SetBackgroundColour(wxColour(255, 255, 255));
		inputField->SetForegroundColour(wxColour(0, 0, 0));
		checklistBox->SetBackgroundColour(wxColour(255, 255, 255));
		checklistBox->SetForegroundColour(wxColour(0, 0, 0));
		clearButton->SetBackgroundColour(wxColour(255, 255, 255));
		clearButton->SetForegroundColour(wxColour(0, 0, 0));
		infoButton->SetBackgroundColour(wxColour(255, 255, 255));
		infoButton->SetForegroundColour(wxColour(0, 0, 0));
		changeColorButton->SetBackgroundColour(wxColour(255, 255, 255));
		changeColorButton->SetForegroundColour(wxColour(0, 0, 0));
	}
	else { //Dunkler modus
		// Farben für den zweiten Modus
		panel->SetBackgroundColour(wxColour(115, 112, 112));
		addButton->SetBackgroundColour(wxColour(51, 51, 51));
		addButton->SetForegroundColour(wxColour(255, 255, 255));
		inputField->SetBackgroundColour(wxColour(51, 51, 51));
		inputField->SetForegroundColour(wxColour(255, 255, 255));
		checklistBox->SetBackgroundColour(wxColour(51, 51, 51));
		checklistBox->SetForegroundColour(wxColour(51, 51, 51));
		clearButton->SetBackgroundColour(wxColour(51, 51, 51));
		clearButton->SetForegroundColour(wxColour(255, 255, 255));
		infoButton->SetBackgroundColour(wxColour(51, 51, 51));
		infoButton->SetForegroundColour(wxColour(255, 255, 255));
		changeColorButton->SetBackgroundColour(wxColour(51, 51, 51));
		changeColorButton->SetForegroundColour(wxColour(255, 255, 255));
	}
	colorMode = !colorMode; // Wechselt den Modus

	panel->Refresh(); // Aktualisieren das Panel, um die neuen Farben anzuzeigen
}

void MainFrame::OnCheckListBoxToggled(wxCommandEvent& evt)
{
	int index = evt.GetInt(); // Index des umgeschalteten Items

	// Überprüfen, ob das Item markiert wurde und nicht einfach unmarkiert wurde
	if (checklistBox->IsChecked(index))
	{
		// Löschen des markierten Items
		checklistBox->Delete(index);
	}
}

void MainFrame::DeleteTask(int index)
{
	// Sicherstellen, dass der Index gültig ist
	if (index >= 0 && index < checklistBox->GetCount())
	{
		checklistBox->Delete(index);
	}
}

// Task hinzufügen
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

// Task löschen
void MainFrame::DeleteSelectedTasks()
{
	int selectedIndex = checklistBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}
	checklistBox->Delete(selectedIndex);
}

// Task verschieben
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

// Task tauschen
void MainFrame::SwapTasks(int i, int j)
{
	Task taskI{ checklistBox->GetString(i).ToStdString(), checklistBox->IsChecked(i) };
	Task taskJ{ checklistBox->GetString(j).ToStdString(), checklistBox->IsChecked(j) };

	checklistBox->SetString(i, taskJ.description);
	checklistBox->Check(i, taskJ.done);

	checklistBox->SetString(j, taskI.description);
	checklistBox->Check(j, taskI.done);
}

// Task umbenennen
void MainFrame::RenameTask(int index)
{
	wxString description = checklistBox->GetString(index);
	wxTextEntryDialog dialog(this, "Beschreibung:", "Task umbennen", description);
	if (dialog.ShowModal() == wxID_OK)
	{
		checklistBox->SetString(index, dialog.GetValue());
	}
}

void MainFrame::DeleteTaskByTaskDone(bool done)
{
	for (int i = checklistBox->GetCount() - 1; i >= 0; i--)
	{
		if (checklistBox->IsChecked(i) == done)
		{
			checklistBox->Delete(i);
		}
	}
}
