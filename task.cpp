#include "task.h"

Task::Task(std::vector<std::string>& parameters) {
		title = parameters[0];
		description = parameters[1];
		date = parameters[2];
		category = parameters[3];
		status = "new";
		std::cout << "Task " << title << " successfully added" << std::endl;
}

Task::Task(const Task& tempTask) :
		title{ tempTask.title },
		description{ tempTask.description },
		date{ tempTask.date },
		category{ tempTask.category } {	}

	const std::string& Task::getTitle() const {
		return title;
    }
	const std::string& Task::getDescription() const {
		return description;
    }
	const std::string& Task::getDate() const {
		return date;
    }
	const std::string& Task::getCategory() const {
		return category;
	}
	const std::string& Task::getStatus() const {
		return status;
	}

	void Task::setTitle(const std::string& title) {
		this->title = title;
		std::cout << "New title : " << title << std::endl;
	}
	void Task::setDescription(const std::string& description) {
		this->description = description;
		std::cout << "New description : " << description << std::endl;
	}
	void Task::setDate(const std::string& date) {
		this->date = date;
		std::cout << "New date : " << date << std::endl;
	}
	void Task::setCategory(const std::string& category) {
		this->category = category;
		std::cout << "New category: " << category << std::endl;
	}
	void Task::setStatus(const std::string& status) {
		this->status = status;
		std::cout << "New status : " << status << std::endl;
	}

	std::string Task::operator()(const std::string& field) const {
		if (field == "title") return this->getTitle();
		if (field == "description") return this->getDescription();
		if (field == "date") return this->getDate();
		if (field == "category") return this->getCategory();
		if (field == "status") return this->getStatus();
		return std::string("Unknown field");
	}

	Task::~Task() {}