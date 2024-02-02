#pragma once
#include <string>
#include <vector>

class Task
{
	std::string description;
	bool done;
};

void saveTasksToFile(const std::vector<Task>& tasks, const std::string& filename);
std::vector<Task> loadTasksFromFile(const std::string& filename);