#include <string>
#include <vector>
#include <iostream>

class Task {
	std::string title;
	std::string description;
	std::string date;
	std::string category;
	std::string status;
public:
	Task() = default;
	Task(std::vector<std::string>& parameters);
	Task& operator=(const Task&) = delete;
	Task& operator=(Task&&) noexcept = delete;

	Task(const Task& tempTask);

	const std::string& getTitle() const;
	const std::string& getDescription() const;
	const std::string& getDate() const;
	const std::string& getCategory() const;
	const std::string& getStatus() const;

	void setTitle(const std::string& title);
	void setDescription(const std::string& description);
	void setDate(const std::string& date);
	void setCategory(const std::string& category);
	void setStatus(const std::string& status);

	std::string operator()(const std::string& field) const;

	~Task();
};