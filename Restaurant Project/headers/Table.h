#pragma once
#include "libraries.h"

class Table {
private:
	bool free;
	int id;
	vector<string> order;
	string name;
public:
	static int tableID;
	Table()
		: free(true), id(tableID++)
	{}

	void SetOrder(vector<string>& order){
		this->order.clear();
		for (int i = 0; i < order.size(); i++)
			(this->order).push_back(order[i]);
	}

	void SetName(string name) {
		this->name = name;
	}

	void SetFree(bool free) {
		this->free = free;
	}
	void SetID(int id) {
		this->id = id;
	}


	string GetName() const { return name; }

	vector<string> GetOrder() const { return order; }

	bool GetID() const { return id; }

	bool IsFree() const { return free; }
};

int Table::tableID = 0;