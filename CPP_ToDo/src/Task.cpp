#include "header/Task.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>


// Save and Load Tasks
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& filename)
{
	std::ofstream ostream(filename);
	ostream << tasks.size();

	for (const Task& task : tasks)
	{
		std::string description = task.description;
		std::replace(description.begin(), description.end(), ' ', '_');

		ostream << '\n' << description << ' ' << task.done;
	}
}
std::vector<Task> LoadTasksFromFile(const std::string& filename)
{
	if (!std::filesystem::exists(filename))
	{
		return std::vector<Task>();
	}
	std::vector<Task> tasks;
	std::ifstream istream(filename);
	int n;
	istream >> n;

	for (int i = 0; i < n; i++)
	{
		std::string description;
		bool done;

		istream >> description >> done;
		std::replace(description.begin(), description.end(), '_', ' ');
        tasks.push_back(Task{ description, done });
	}
	return tasks;
}


// Save and Load Done Tasks
void SaveDoneTasksToFile(const std::vector<Task>& tasks, const std::string& filename)
{
	std::ofstream ostream(filename);
	ostream << tasks.size();

	for (const Task& task : tasks)
	{
		std::string description = task.description;
		std::replace(description.begin(), description.end(), ' ', '_');

		ostream << '\n' << description << ' ' << task.done;
	}
}
std::vector<Task> LoadDoneTasksFromFile(const std::string& filename)
{
	if (!std::filesystem::exists(filename))
	{
		return std::vector<Task>();
	}
	std::vector<Task> tasks;
	std::ifstream istream(filename);
	int n;
	istream >> n;

	for (int i = 0; i < n; i++)
	{
		std::string description;
		bool done;

		istream >> description >> done;
		std::replace(description.begin(), description.end(), '_', ' ');
		tasks.push_back(Task{ description, done });
	}
	return tasks;
}