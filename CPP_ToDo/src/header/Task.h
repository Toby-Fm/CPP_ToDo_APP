#pragma once
#include <string>
#include <vector>

struct Task
{
	std::string description;
	bool done;
};

// Save and Load Tasks
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
std::vector<Task> LoadTasksFromFile(const std::string& filename);

// Save and Load Done Tasks
void SaveDoneTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
std::vector<Task> LoadDoneTasksFromFile(const std::string& filename);