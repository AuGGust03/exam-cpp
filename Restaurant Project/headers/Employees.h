#pragma once
#include "Employee.h"
#include "Database.h"
#include "Display.h"
#include "Table.h"

using namespace display;

static vector<Table> tables;

class Cook : public Employee {
private:
	vector<string> orderList;
public:
	Cook()
		:Employee() 
	{}

	Cook(string name, int id, int salary, string password)
		:Employee(name, id, salary, password, "Cook")
	{}

	void Menu() override {
		vector<string> menuP;

		menuP.push_back("Check orders");
		menuP.push_back("Add dish to menu");
		menuP.push_back("Remove dish from menu");
		menuP.push_back("Change dish in menu");
		menuP.push_back("Logout");

		bool logout = 0;


		while (!logout) {
			unsigned int choice = PrintMenu(menuP, "ID - " + to_string(id) + "\n\nCook menu");
			string type;

			switch (choice) {
			case 0:
				CheckOrders();
				break;
			case 1:
				AddDishMenu();
				break;

			case 2:
				RemoveDishMenu();
				break;

			case 3:
				ChangeDishMenu();
				break;

			case 4:
				logout = 1;
				break;
			}
		}
	}

	void AddDishMenu() {
		string name = GetData<string>("Name of dish: ");
		int price = GetData<int>("Price of dish: ");
		vector<string> menuP;
		menuP.push_back("Ok");
		menuP.push_back("Cancel");
		bool choise = PrintMenu(menuP, "Add?\n" + name + " - " + to_string(price));

		if (!choise) {
			menu.emplace(name, price);
			db.AddDish(name, price);
		}
	}

	void RemoveDishMenu() {
		vector<string> menuP;

		int choice = ChooseDish();
		if (choice >= menu.size())
			return;

		auto it = menu.begin();
		for (int i = 0; i < choice; i++)
			it++;
		db.RemoveDish(it->first);
		menu.erase(it);
	}

	void ChangeDishMenu() {
		vector<string> menuP;

		int choice = ChooseDish();
		if (choice >= menu.size())
			return;

		auto it = menu.begin();
		for (int i = 0; i < choice; i++)
			it++;

		int newPrice = GetData<int>("New price: ");

		menu[it->first] = newPrice;

		db.ChangeDish(it->first, newPrice);
	}

	void CheckOrders() {
		vector<string> menuP;
		vector<int> ids;
		for (int i = 0; i < tables.size(); i++) {
			if (tables[i].IsFree())
				continue;
			int orderSize = tables[i].GetOrder().size();
			if (orderSize == 0)
				continue;
			string data;
			vector<string> tempData = tables[i].GetOrder();
			for (int j = 0; j < orderSize; j++)
				data += tempData[j] + ',';
			data[data.size() - 1] = ' ';
			menuP.push_back(to_string(i) + " table  - " + data );
			ids.push_back(i);
		}
		menuP.push_back("Cancel");

		int choice = PrintMenu(menuP, "Select an order if it's ready: ");

		if (choice == menuP.size() - 1)
			return;
		RemoveOrder(ids[choice]);
	}

	void RemoveOrder(int id) {
		vector<string> temp;
		tables[id].SetOrder(temp);
		db.RemoveOrder(id);
	}

	static int  ChooseDish() {
		vector<string> dishes;

		for (auto it = menu.begin(); it != menu.end(); it++)
			dishes.push_back(it->first + " - " + to_string(it->second));
		dishes.push_back("Back");

		int choice = PrintMenu(dishes, "Please, choice dish: ");

		return choice;
	}
};

class Waiter : public Employee {
public:
	Waiter():Employee() {}

	Waiter(string name, int id, int salary, string password)
		:Employee(name, id, salary, password, "Waiter")
	{}

	void Menu() override{
		vector<string> menuP;

		menuP.push_back("Add order");
		menuP.push_back("Empty table");
		menuP.push_back("Logout");

		bool logout = 0;


		while (!logout) {
			unsigned int choice = PrintMenu(menuP, "ID - " + to_string(id) + "\n\nWaiter menu");

			switch (choice) {
			case 0:
				AddOrder();
				break;
			case 1:
				EmptyTable();
				break;

			case 2:
				logout = 1;
				break;
			}
		}

	}

	void EmptyTable() {
		int tNum = GetData<int>("Table num: ");
		if (tNum >= tables.size()) {
			PrintMenu(vector<string>{"Ok"}, "There is not " + to_string(tNum) + " table");
			return;
		}
		if (tables[tNum].IsFree()) {
			PrintMenu(vector<string>{"Ok"}, "This table is free!");
			return;
		}
		tables[tNum].SetFree(1);
		vector<string> temp;
		tables[tNum].SetOrder(temp);
		tables[tNum].SetName(string());
		db.EmptyTable(tNum);
	}

	void AddOrder() {
		int tNum = GetData<int>("Table num: ");
		if (tNum >= tables.size()) {
			PrintMenu(vector<string>{"Ok"}, "There is not " + to_string(tNum) + " table");
			return;
		}
		if (tables[tNum].IsFree()) {
			PrintMenu(vector<string>{"Ok"}, "This table is free!");
			return;
		}
			
		vector<string> menuP;

		menuP.push_back("Add dish");
		menuP.push_back("Remove dish");
		menuP.push_back("Ok");
		menuP.push_back("Cancel");
		
		vector<string> dishList;
		int choise = 0;

		while (true){
			string pD = "Order:\n";
			for (int i = 0; i < dishList.size(); i++)
				pD += dishList[i] + " - " + to_string(menu[dishList[i]]) + "\n";
			choise = PrintMenu(menuP, pD);
			
			switch (choise) {
			case 0:
				AddDish(dishList);
				break;
			case 1:
				RemoveDish(dishList);
				break;
			case 2:
				tables[tNum].SetOrder(dishList);
				db.AddOrder(tNum, dishList);
				return;
			case 3:
				return;
			}
		}
	}

	void AddDish(vector<string>& dishList) {
		int choise = Cook::ChooseDish();

		if (choise >= menu.size())
			return;

		auto it = menu.begin();
		for (int i = 0; i < choise; i++)
			it++;

		string dish = it->first;
		dishList.push_back(dish);
	}

	void RemoveDish(vector<string>& dishList) {
		vector<string> menuP = dishList;
		menuP.push_back("Cancel");
		int choise = PrintMenu(menuP, "Choose dish: ");

		if (choise >= dishList.size())
			return;

		dishList.erase(dishList.begin() + choise);
	}

};

class Administartor : public Employee{
private:
public:
	Administartor():Employee() {}

	Administartor(string name, int id, int salary, string password)
		:Employee(name, id, salary, password, "Administrator")
	{}

	void Menu() override {
		vector<string> menuP;
		string idPos = "ID - " + to_string(id) + "\n\nAdministrator menu";

		menuP.push_back("Reserve table");
		menuP.push_back("Empty reserved table");
		menuP.push_back("Logout");

		bool logout = 0;


		while (!logout) {
			int freeTables = 0;
			for (int i = 0; i < tables.size(); i++) {
				if (tables[i].IsFree())
					freeTables++;
			}
			string title = idPos + "\n\nFree tables: " + to_string(freeTables);
			unsigned int choice = PrintMenu(menuP, title);
			string type;

			switch (choice) {
			case 0:
				ReserveTable();
				break;
			case 1:
				EmptyTable();
				break;

			case 2:
				logout = 1;
				break;
			}
		}

	}

	void EmptyTable() {
		vector<string> menuP;
		vector<int> ids;
		for (int i = 0; i < tables.size(); i++) {
			if (!tables[i].IsFree()) {
				if (tables[i].GetOrder().size() == 0) {
					string strID = "??";//to_string(tables[i].GetID());
					string data =  strID + " - " + tables[i].GetName();
					menuP.push_back(data);
					ids.push_back(i);
				}	
			}
		}
		menuP.push_back("Back");

		int choice = PrintMenu(menuP, "Chooce the reserver table:");
		if (choice == menuP.size() - 1)
			return;
		
		tables[ids[choice]].SetFree(1);
		tables[ids[choice]].SetName("");
		db.EmptyTable(ids[choice]);
	}

	void ReserveTable() {
		int i = 0;
		for (i; i < tables.size(); i++) {
			if (tables[i].IsFree())
				break;
		}
		if (i >= tables.size()) {
			PrintMenu(vector<string>{"Ok"}, "There is not free table");
			return;
		}
		string name = GetData<string>("Name of reserver: ");
		tables[i].SetFree(0);
		tables[i].SetName(name);
		db.EmptyTable(i, 0, name);
	}
};


class Director : public Employee {
private:
	static int ID;
public:
	static vector<int> freeID;
	Director() :Employee() {}

	Director(string name, int id, int salary, string password)
		:Employee(name, id, salary, password, "Director")
	{}

	void Menu(vector<Employee*>& emps) {
		vector<string> menu;

		menu.push_back("Add Employee");
		menu.push_back("Edit Employee");
		menu.push_back("Remove Employee");
		menu.push_back("Discount system");
		menu.push_back("Edit tables");
		menu.push_back("Logout");

		bool logout = 0;
		
		while (!logout) {
			unsigned int choice = PrintMenu(menu, "ID - " + to_string(id) + "\n\nDirector menu");
			string type;

			switch (choice) {
			case 0:
				type = ChoosePos();
				if (type == "Cancel")
					break;
				cout << "Registration of a new " << type << ':' << endl << endl;
				AddEmp(emps, type);
				break;

			case 1:
				choice = ChooseEmp(emps);
				if (choice >= emps.size())
					break;
				cout << "What data do you need to change? " << endl << endl;
				type = ChooseData();
				if (type == "Cancel")
					break;
				EditEmp(emps, choice,type);
				break;

			case 2:
				choice = ChooseEmp(emps);
				if (choice >= emps.size())
					break;
				RemoveEmp(emps, choice);
				break;

			case 3:
				Discount();
				break;

			case 4:
				EditTables();
				break;

			case 5:
				logout = 1;
				break;
			}
		}
	}

	void EditTables() {
		vector<string> menuP;
		menuP.push_back("Add table");
		menuP.push_back("Remove table");
		menuP.push_back("Back");

		int choice = int();

		while (choice != 2) {
			choice = PrintMenu(menuP, "Number of tables: " + to_string(tables.size()));
			if (choice == 0) 
				AddTable();
			else if (choice == 1) 
				RemoveTable();
		}	
	}

	static void AddTable() {
		tables.push_back(Table());
		db.AddTable(Table::tableID-1);
	}

	void RemoveTable() {
		int size = tables.size();
		if (size == 0) {}
		else {
			if (!tables[size - 1].IsFree())
				PrintMenu(vector<string>{"ok"}, "All tables is not free");

			else {
				tables.erase(tables.begin() + (size - 1));
				db.RemoveTable();
			}
		}
	}


	void Discount() {
		vector<string> menu;
		menu.push_back("Add new Discount");
		menu.push_back("Change Discount");
		menu.push_back("Remove Discount");
		menu.push_back("Cancel");

		int choice = PrintMenu(menu, "Discount system: ");

		if (choice == 0) {
			int key, value;
			int bef = discounts.size();
			cout << "Adding a new discount" << endl << endl;
			key = GetData<int>("Amount spent to get a discount: ");
			value = GetData<int>("Amount of discount: ");
			discounts.emplace(key, value);
			if (discounts.size() == bef)
				PrintMenu(vector<string>{"Ok"}, "This discount already exists!");
			else
				db.AddDiscount(key, value);
		}
		else if (choice == 1) {
			choice = ChooseDiscount();
			auto it = discounts.begin();
			for (int i = 0; i < choice; i++)
				it++;
			if (choice >= discounts.size())
				return;
			EditDiscount(it);
		}
		else if(choice == 2){
			choice = ChooseDiscount();
			auto it = discounts.begin();
			for (int i = 0; i < choice; i++)
				it++;
			if (choice >= discounts.size())
				return;
			RemoveDiscount(it);
		}
		else 
			return;
		
		
	}

	void RemoveDiscount(map<int, int>::iterator& it) {
		db.RemoveDiscountDB(it->first);
		discounts.erase(it);
	}

	void EditDiscount(map<int, int>::iterator& it) {
		int nData = GetData<int>("New amount of discount: ");
		int key = it->first;
		it->second = nData;
		db.EditDiscountDB(key, nData);

	}

	static void AddEmp(vector<Employee*>& employees, string position, vector<string>* data = nullptr) {
		int id = int();
		string name, password;
		int salary = int();

		if (data == nullptr) {
			if (freeID.size() == 0)
				id = ID++;
			else {
				auto it = min(freeID);
				id = *it;
				freeID.erase(it);
			}
			cout << "ID of new " << position << " " << id << ", it is needed to enter\n\n";
			name = GetData<string>(position + " name: ");
			salary = GetData<int>(position + " salary: ");
			password = Pass("Password for " + position + " account: ");
		}
		else {
			id = stoi((*data)[0]);
			ID = id + 1;
			position = (*data)[1];
			password = (*data)[2];
			name = (*data)[3];
			salary = stoi((*data)[4]);
		}

		

		if (position == "Director")
			employees.push_back(static_cast<Director*>(new Director(name, id, salary, password)));
		else if (position == "Cook")
			employees.push_back(static_cast<Cook*>(new Cook(name, id, salary, password)));
		else if (position == "Waiter")
			employees.push_back(static_cast<Waiter*>(new Waiter(name, id, salary, password)));
		else
			employees.push_back(static_cast<Administartor*>(new Administartor(name, id, salary, password)));
		sort(employees.begin(), employees.end(), [](Employee* first, Employee* second) {
			return first->GetId() < second->GetId();
			});
		if (data == nullptr) {
			string row = ToRow(employees[Find(employees, id)]);
			if (id >= ID - 1)
				id = 0;
			db.AddEmpBD(row, id);
		}
			
	}

	void EditEmp(vector<Employee*>& emps, int index, string type) {
		string typeData;
		if (type == "Salary") {
			typeData = to_string(GetData<int>("New " + type + ": "));
			emps[index]->SetSalary(stoi(typeData));
		}
			
		else {
			
			if (type == "Name") {
				typeData = GetData<string>("New " + type + ": ");
				emps[index]->SetName(typeData);
			}
			else {
				typeData = Pass("New " + type + ": ");
				emps[index]->SetPassword(typeData);
			}
		}
			
		db.EditEmpBD(index, type, typeData);
	}

	void RemoveEmp(vector<Employee*>& emps, int index) {
		if (index == 0) {
			PrintMenu(vector<string>{"Ok"}, "Can't remove general director");
			return;
		}
		emps.erase(emps.begin() + index);
		freeID.push_back(index);
		db.RemoveEmpBD(index);
	}

	string ChoosePos() {
		vector<string> positions;
		positions.push_back("Director");
		positions.push_back("Cook");
		positions.push_back("Waiter");
		positions.push_back("Administrator");
		positions.push_back("Cancel");

		int choice = PrintMenu(positions, "Please, choice positions: ");

		return positions[choice];
	}

	int ChooseEmp(vector<Employee*>& emps) {
		vector<string> menu;
		string temp;

		menu.clear();

		for (int i = 0; i < emps.size(); i++) {
			;
			temp = Join(vector<string>{ emps[i]->GetPosition(), to_string(emps[i]->GetId()),
				emps[i]->GetName(), to_string(emps[i]->GetSalary()) }, " | ");
			menu.push_back(temp+"\n");
		}
		menu.push_back("Cancel");
		return PrintMenu(menu, "Choose Employer:");
	}

	string  ChooseData() {
		vector<string> types;
		types.push_back("Name");
		types.push_back("Salary");
		types.push_back("Password");
		types.push_back("Cancel");

		int choice = PrintMenu(types, "Please, choice positions: ");

		return types[choice];
	}

	int  ChooseDiscount() {
		vector<string> types;
		for (auto it = discounts.begin(); it != discounts.end(); it++)
			types.push_back(to_string(it->first) + " - " + to_string(it->second) + "%");

		types.push_back("Cancel");

		int choice = PrintMenu(types, "Please, choice discount: ");

		return choice;
	}

};
int Director::ID = 0;
vector<int> Director::freeID = vector<int>();