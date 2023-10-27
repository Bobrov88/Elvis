#include <map>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include "task.h"

constexpr auto SPACE = ' ';
constexpr auto tab = '\t';

class TaskManager {
	std::map<std::string, std::function<void()>> command;
	std::map<std::string, Task> tasks;
	std::vector<std::string> parameters;
public:
	TaskManager();

	static bool comparator(const Task& task,
		const std::string& field,
		const std::string& ops,
		const std::string& data);
	void ReadingData(std::fstream& file_);
    void ReadingFile(const char* file_);
	bool isStringValid(const std::string& str) const;
	bool isParameterValid(const size_t n) const;
	void printAll() const;
	void stringToLowerCase(std::string& str);
	void addTask();
	void doneTask();
	bool updateHelper(const char* field);
	void updateTask();
	void deleteTask();
	std::string selectHelper(const std::string& str) const;
	void selectTask();

	~TaskManager();
};
