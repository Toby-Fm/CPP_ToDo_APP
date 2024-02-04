#include "header/main.h"
#include "header/Task.h"
#include <vector>
#include <string>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title), colorMode(false)
{
	// Initialisiert die Benutzeroberfläche und bindet Ereignisbehandler.
	CreateControls();
	BindEventHandlers();
	AddSavedTasks();
}

// Erstellt und platziert die Steuerelemente im Hauptfenster.
void MainFrame::CreateControls()
{
	// Definition der Schriftarten für die Überschrift und andere Texte.
	wxFont healineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	// Erstellung des Hauptpanels und Festlegung der Schriftart.
	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	// Erstellung der Benutzeroberflächenelemente.
	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add");
	checklistBox = new wxCheckListBox(panel, wxID_ANY);
	checklistBox->SetFont(mainFont);
	clearButton = new wxButton(panel, wxID_ANY, "Clear");
	changeColorButton = new wxButton(panel, wxID_ANY, "Change Color");
	infoButton = new wxButton(panel, wxID_ANY, "Info");

	// Erstellung und Konfiguration der Layout-Sizer.
	wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hSizer2 = new wxBoxSizer(wxHORIZONTAL);

	// Hinzufügen der Elemente zu den Sizern und Festlegung ihrer Eigenschaften.
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

	// Anwendung der Sizer auf das Panel und Aktualisierung des Layouts.
	panel->SetSizer(vSizer);
	panel->Layout();
}

// Bindet Funktionen an Ereignisse für die Steuerelemente.
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

// Lädt gespeicherte Aufgaben aus einer Datei und fügt sie zur Checkliste hinzu.
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

// Ereignisbehandler für das Klicken auf die Schaltfläche "Add".
void MainFrame::OnAddButtonClicked(wxCommandEvent& event)
{
	AddTaskFromInput();
}

// Ereignisbehandler für das Drücken der Eingabetaste im Eingabefeld.
void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

// Ereignisbehandler für das Drücken von Tasten in der Checkliste.
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

// Ereignisbehandler für das Klicken auf die Schaltfläche "Clear".
void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	// Überprüfen, ob die Checkliste leer ist.
	if (checklistBox->IsEmpty())
	{
		return;
	}

	// Anzeigen einer Bestätigungsdialogbox.
	wxMessageDialog dialog(this, "Möchtest du wirklich alle Tasks löschen?", "Löschen", wxYES_NO | wxCANCEL | wxICON_QUESTION);
	int result = dialog.ShowModal();
	if (result == wxID_YES)
	{
		checklistBox->Clear();
	}

}

// Ereignisbehandler für das Schließen des Hauptfensters.
void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	// Speichern der Aufgaben in einer Datei.
	std::vector<Task> tasks;
	for (int i = 0; i < checklistBox->GetCount(); i++)
	{
		// Erstellen einer Task-Struktur für jedes Element in der Checkliste.
		Task task;
		task.description = checklistBox->GetString(i);
		task.done = checklistBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTasksToFile(tasks, "tasks.txt");
	evt.Skip();
}

// Ereignisbehandler für das Klicken auf die Schaltfläche "Info".
void MainFrame::OnInfoButtonClicked(wxCommandEvent& evt)
{
	wxMessageBox(InfoText, "Info", wxOK | wxICON_INFORMATION);
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

// Ereignisbehandler für das Umschalten eines Elements in der Checkliste.
void MainFrame::OnCheckListBoxToggled(wxCommandEvent& evt)
{
	int index = evt.GetInt(); // Index des umgeschalteten Items

	// Überprüfen, ob das Item markiert wurde
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
	// Beschreibung aus dem Eingabefeld holen
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
	// Sicherstellen, dass ein Element ausgewählt ist
	int selectedIndex = checklistBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}

	// Sicherstellen, dass das neue Index innerhalb des gültigen Bereichs liegt
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
	// Sicherstellen, dass die Indizes gültig sind
	Task taskI{ checklistBox->GetString(i).ToStdString(), checklistBox->IsChecked(i) };
	Task taskJ{ checklistBox->GetString(j).ToStdString(), checklistBox->IsChecked(j) };

	// Tauschen der Tasks
	checklistBox->SetString(i, taskJ.description);
	checklistBox->Check(i, taskJ.done);

	checklistBox->SetString(j, taskI.description);
	checklistBox->Check(j, taskI.done);
}

// Task umbenennen
void MainFrame::RenameTask(int index)
{
	// Sicherstellen, dass der Index gültig ist
	wxString description = checklistBox->GetString(index);
	wxTextEntryDialog dialog(this, "Beschreibung:", "Task umbennen", description);
	if (dialog.ShowModal() == wxID_OK)
	{
		checklistBox->SetString(index, dialog.GetValue());
	}
}

// Task löschen wenn sie als erledigt markiert sind
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
