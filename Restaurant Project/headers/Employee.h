#pragma once
#include "Person.h"

class Employee : public Person {
protected:
	string position;
	string password;
	int salary;
public:
	Employee()
		:Person() {
		position = string();
		salary = int();
	}

	Employee(string name, int id, int salary, string password, string position)
		:Person(name, id), salary(salary), password(password), position(position)
	{}

	virtual void Menu(){}

	int GetSalary() const { return salary;}

	string GetPosition() const { return position; }

	string GetPassword() const { return password; }

	void SetPassword(string password) { this->password = password; }

	void SetSalary(int salary) { this->salary =  salary; }

	bool operator >(Employee& second) {
		return id > second.id;
	}
	
	bool operator <(Employee& second) {
		return id < second.id;
	}

	bool operator ==(Employee& second) {
		return id == second.id;
	}

	bool operator ==(int second) {
		return id == second;
	}

};

string ToRow(Employee*& emp) {
	string row;
	row += to_string(emp->GetId());
	row += ' ';
	row += emp->GetPosition();
	row += ' ';
	row += emp->GetPassword();
	row += ' ';
	row += emp->GetName();
	row += ' ';
	row += to_string(emp->GetSalary());
	return row;
}

int Find(vector<Employee*>& emps, int id) {
	for (int i = 0; i < emps.size(); i++) {
		if (emps[i]->GetId() == id)
			return i;
	}
	return -1;
}