#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	bool colorMode;

	void CreateControls();
	void BindEventHandlers();
	void AddSavedTasks();
	
	void OnAddButtonClicked(wxCommandEvent& evt);
	void OnInputEnter(wxCommandEvent& evt);
	void OnListKeyDown(wxKeyEvent& evt);
	void OnClearButtonClicked(wxCommandEvent& evt);
	void OnWindowClosed(wxCloseEvent& evt);
	void OnInfoButtonClicked(wxCommandEvent& evt);
	void OnChangeColorButtonClicked(wxCommandEvent& evt);
	void OnCheckListBoxToggled(wxCommandEvent& evt);
	void DeleteTask(int index);

	void AddTaskFromInput();
	void DeleteSelectedTasks();
	void MoveSelectedTasks(int offset);
	void SwapTasks(int i, int j);
	void RenameTask(int index);
	void DeleteTaskByTaskDone(bool done);

	wxPanel* panel;
	wxStaticText* headlineText;
	wxTextCtrl* inputField;
	wxButton* addButton;
	wxCheckListBox* checklistBox;
	wxButton* clearButton;
	wxButton* changeColorButton;
	wxButton* infoButton;

};