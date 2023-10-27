#include "taskmanager.h"

std::ostream& operator<<(std::ostream& os, const Task& task) {
	os << "Title: " << task.getTitle()
		<< " Description: " << task.getDescription()
		<< " Date: " << task.getDate()
		<< " Category: " << task.getCategory()
		<< " Status: " << task.getStatus();
	return os;
}

TaskManager::TaskManager()	{
		command["add"] = std::bind(&TaskManager::addTask, this);
		command["done"] = std::bind(&TaskManager::doneTask, this);
		command["update"] = std::bind(&TaskManager::updateTask, this);
		command["delete"] = std::bind(&TaskManager::deleteTask, this);
		command["select*"] = std::bind(&TaskManager::selectTask, this);
		parameters.reserve(4);
}

void TaskManager::ReadingData(std::fstream& is) {
		for (std::string line; getline(is, line);) {
			if (isStringValid(line)) {
				stringToLowerCase(line);
				std::istringstream iss{ line };
				std::istream_iterator<std::string> _begin{ iss };
				std::istream_iterator<std::string> _end{ };
				std::string comm{ selectHelper(*_begin) };
				if (command.count(comm)) {
					for (auto& it = std::next(_begin); it != _end; ++it)
						parameters.push_back(*it);
					command[comm]();
					parameters.clear();
				}
				else if (*_begin == "exit") break;
				else std::cout << "Unknown command : " << selectHelper(*_begin) << std::endl;
			}
		}
}

	void TaskManager::ReadingFile(const char* file_) {
		std::fstream file;
		file.open(file_, std::ios::in);
		if (!file.is_open()) {
			std::cout << "File " << file_ << " not found!" << std::endl;
			file.close();
		} 
		else ReadingData(file);
    }

	bool TaskManager::isStringValid(const std::string& str) const {
		return (!str.empty() && str.find_first_not_of(SPACE) != std::string::npos);
	}

	bool TaskManager::isParameterValid(const size_t n) const {
		if (parameters.size() < n)
		{
			std::cout << "Too few parameters" << std::endl;
			return false;
		}
		if (parameters.size() > n) {
			std::cout << "Too many parameters" << std::endl;
			return false;
		}
		return true;
    }

	void TaskManager::printAll() const {
		for (const auto& t : tasks) {
			std::cout << t.second;
		}
    }

	void TaskManager::stringToLowerCase(std::string& str) {
		std::transform(std::begin(str),
			std::end(str),
			std::begin(str),
			::tolower);
	}

	void TaskManager::addTask() {
		if (isParameterValid(4)) {
			if (tasks.find(parameters[0]) == tasks.end())
				tasks.emplace(parameters[0], parameters);
			else std::cout << "Task " << parameters[0] << " already exists" << std::endl;
		}
	}

	void TaskManager::doneTask() {
		if (isParameterValid(1)) {
			if (tasks.find(parameters[0]) != tasks.end()) {
				tasks[parameters[0]].setStatus("done");
			}
			else std::cout << "Task " << parameters[0] << " not found" << std::endl;
		}
	}

	bool TaskManager::updateHelper(const char* field) {
		std::cout << tab << field << ": ";
		getline(std::cin, parameters[0]);
		if (isStringValid(parameters[0]))
			return true;
		return false;
	}

	void TaskManager::updateTask() {
		if (isParameterValid(1)) {
			const std::string updatetask{ parameters[0] };
			if (tasks.find(parameters[0]) != tasks.end())
			{
				std::cout << parameters[0] << " -> ";
				if (updateHelper("Title")) {
					tasks.insert({parameters[0], {}});
					tasks.erase(updatetask);				
					std::cout << tab;
					tasks[parameters[0]].setTitle(parameters[0]);
				}
				if (updateHelper("Description")) {
					std::cout << tab;
					tasks[parameters[0]].setDescription(parameters[0]);
				}
				if (updateHelper("Date")) {
					std::cout << tab;
					tasks[parameters[0]].setDate(parameters[0]);
				}
				if (updateHelper("Category")) {
					std::cout << tab;
					tasks[parameters[0]].setCategory(parameters[0]);
				}
				if (updateHelper("Status")) {
					std::cout << tab;
					tasks[parameters[0]].setStatus(parameters[0]);
				}
			}
			else std::cout << "Task " << updatetask << " not found" << std::endl;
		}
    }

	void TaskManager::deleteTask() {
		if (isParameterValid(1)) {
			if (tasks.find(parameters[0]) != tasks.end()) {
				tasks.erase(parameters[0]);
				std::cout << "Task \'" << parameters[0] << "\' successfully terminated" << std::endl;
			}
			else std::cout << "Task " << parameters[0] << " not found" << std::endl;
		}
    }

	std::string TaskManager::selectHelper(const std::string& str) const {
		if ((str.find("select") != std::string::npos &&
			str.find('*') != std::string::npos &&
			str.find('*') < str.find("where")) ||
			str.find("select") != std::string::npos)
			return std::string{ "select*" };
		return str;
    }

	void TaskManager::selectTask() {

		std::string condition;
		std::vector<std::string> fields, ops, datas, ands;

		for (auto& i : parameters)
			condition += i;

		std::regex field{ "(title|description|date|category|status)" };
		for (std::sregex_iterator iBegin{ condition.begin(),condition.end(),field }, iEnd;
			iBegin != iEnd;
			++iBegin)
			fields.push_back(iBegin->str());

		std::regex op{ "(<=|>=|!=|<|>|=|like)" };
		for (std::sregex_iterator iBegin{ condition.begin(),condition.end(),op }, iEnd;
			iBegin != iEnd;
			++iBegin)
			ops.push_back(iBegin->str());

		std::regex data{ "\"[0-9a-z :_-]+\"" };
		for (std::sregex_iterator iBegin{ condition.begin(),condition.end(),data }, iEnd;
			iBegin != iEnd;
			++iBegin) {
			datas.push_back(iBegin->str().substr(
				iBegin->str().find_first_not_of('\"'),
				iBegin->str().find_last_not_of('\"') -
				iBegin->str().find_first_not_of('\"') + 1));
		}

		std::regex and_{ "\"[ ]?(and)[ ]?(title|description|date|category|status)" };
		for (std::sregex_iterator iBegin{ condition.begin(),condition.end(),and_ }, iEnd;
			iBegin != iEnd;
			++iBegin)
			ands.push_back(iBegin->str());

		if (((fields.size() != ops.size() ||
			fields.size() != datas.size() ||
			ops.size() != datas.size()) ||
			ands.size() != fields.size() - 1) &&
			condition.length() != 1) {
			std::cout << "Invalid parameters" << std::endl;
			return;
		}

		if (fields.size() == 0)
		{
			for (auto& task : tasks)
				std::cout << task.second << std::endl;
			return;
		}

		bool isSatisfiesToConditions{ 0 };
		for (const auto& task : tasks) {
			isSatisfiesToConditions = true;
			for (size_t i{ 0 }; i < fields.size(); ++i) {
				if (!comparator(task.second, fields[i], ops[i], datas[i])) {
					isSatisfiesToConditions = false;
					break;
				}
			}
			if (isSatisfiesToConditions)
				std::cout << task.second << std::endl;
        }
    }

	bool TaskManager::comparator(const Task& task,
		const std::string& field,
		const std::string& ops,
		const std::string& data) {
		if (ops == "<=") return task(field) <= data;
		if (ops == "!=") return task(field) != data;
		if (ops == ">=") return task(field) >= data;
		if (ops == "=") return task(field) == data;
		if (ops == "<") return task(field) < data;
		if (ops == ">") return task(field) > data;
		if (ops == "like") return (task(field).find(data) != std::string::npos);
		return false;
        }

	TaskManager::~TaskManager() {}