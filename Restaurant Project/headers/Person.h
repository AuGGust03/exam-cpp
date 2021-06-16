#pragma once
#include "libraries.h"


class Person {
protected:
	string name;
	int id;
public:
	Person() 
		:name(string()), id(int())
	{}

	Person(string name, int id)
		:name(name), id(id)
	{}

	Person(const Person& obj) {
		this->name = obj.name;
		this->id = obj.id;
	}

	string GetName() const { return name; }

	int GetId() const { return id; }

	void SetName(string name) { this->name = name; }	

	void SetId(int id) { this->id = id; }


	virtual ~Person() = 0 {}
};

class Client : public Person {
public:
	Client()
		:Person()
	{}

	Client(string name, int id)
		:Person(name, id)
	{}

};
