#pragma once
#include "Employees.h"

namespace rest {

	using namespace display;

	class Restaurant {
	private:
		vector<Employee*> employees;

		int Auth() {
			cout << "Authorization:\n" << endl;
			int authID = GetData<int>("ID of your account: ");
			while (Find(employees, authID) == -1) {
				cout << "Not found ID, try again" << endl;
				authID = GetData<int>("ID of your account: ");
			}
			string pass = employees[authID]->GetPassword();
			auto getPass = [](auto& self, string pass, int attempts = 0) ->bool{
				
				string authPass = Pass("Password of your account: ");
				if (authPass != pass) {
					if (attempts == 2) {
						PrintMenu(vector<string>{"Change ID"}, "3 wrong attempts, please check ID");
						return 0;
					}
					bool choice = PrintMenu(vector<string>{"Try again", "Change ID"}, "Invalid Password!");
					if (!choice) {
						system("cls");
						return self(self, pass, ++attempts);
					}
					else {
						return 0;
					}
				}
				else
					return 1;
			};
			return getPass(getPass, pass)? authID :-1;
		}
	public:
		Restaurant() {

		}


		bool CheckDB () const{
			return db.Check();
		}

		void Create() {
			employees.clear();
			db.Create();
			Director::AddEmp(employees, "Director");
		}

		bool Import(string path = "database/") {
			bool res = 1;
			if (path != db.GetPath()) {
				if (!db.Import(path))
					return 0;
			}
				
			res = ImportEmp() && res;
			if (!filesystem::exists(db.GetPath() + "discounts.txt")) {
				ofstream temp(db.GetPath() + "discounts.txt");
				temp.close();
			}
			else
				res = ImportDiscounts() && res;
			if (!filesystem::exists(db.GetPath() + "tables.txt")) {
				ofstream temp(db.GetPath() + "tables.txt");
				temp.close();
			}
			else
				res = ImportTables() && res;
			if (!filesystem::exists(db.GetPath() + "menu.txt")) {
				ofstream temp(db.GetPath() + "menu.txt");
				temp.close();
			}
			else
				res = ImportMenu() && res;
			return res;
		}

		bool ImportMenu() {
			ifstream fin(db.GetPath() + "menu.txt");
			while (!fin.eof()) {
				string row;
				getline(fin, row);
				vector<string> data = Split(row);
				if (data.size() < 2)
					continue;
				menu[data[0]] = stoi(data[1]);
			}
			return 1;
		}

		bool ImportTables() {
			ifstream fin(db.GetPath() + "tables.txt");
			while (!fin.eof()) {
				string row;
				getline(fin, row);
				vector<string> data = Split(row);
				if (data.size() < 2)
					continue;
				tables.push_back(Table());
				int i = tables.size() - 1;
				int dSize = data.size();
				if (dSize > 2) {
					tables[i].SetName(data[2]);
					vector<string> order;
					for (int i = 3; i < dSize; i++) {
						order.push_back(data[i]);
					}
					tables[i].SetOrder(order);
				}	
				tables[i].SetFree(stoi(data[1]));
			}
			return 1;
		}

		bool ImportEmp() {
			ifstream fin(db.GetPath() + "employees.txt");
			while (!fin.eof()) {
				string row;
				getline(fin, row);
				vector<string> data = Split(row);
				if (data.size() < 5)
					continue;

				if (employees.size() != 0) {
					auto last = employees.end();
					int lastID = employees[employees.size()-1]->GetId() + 1;
					while (stoi(data[0]) > lastID) {
						lastID++;
						Director::freeID.push_back(lastID);
					}
				}
				Director::AddEmp(employees, string(), &data);
			}
			for (int i = 0; i < employees.size(); i++) {
				int tID = employees[i]->GetId();
				int count = std::count_if(employees.begin(), employees.end(), [tID](Employee*& first) {
					return first->GetId() == tID;
					});
				if (count > 1)
					return 0;
			}
			return 1;
		}

		bool ImportDiscounts() {
			ifstream fin(db.GetPath() + "discounts.txt");
			while (!fin.eof()) {
				string row;
				getline(fin, row);
				vector<string> data = Split(row);
				if (data.size() < 2)
						continue;	
				discounts.emplace(stoi(data[0]), stoi(data[1]));
			}
			return 1;
		}

		int Authorization() {
			int authID = -1;
			do {
				authID = Auth();
			} while (authID == -1);
			return authID;
		}

		bool Menu(int authID) {
			if (authID == 0) 
				static_cast<Director*>(employees[authID])->Menu(employees);
			else
				employees[authID]->Menu();
			return 1;
		}


		bool InsertDB () {
			bool choice = PrintMenu(vector<string>{ "Create new DB", "Import DB" },
									"It is first start, please select DB:"       );
			if (choice) {

				auto getPath = []() ->string {
					string path = GetData<string>("Path to DB: ");
					if (path[path.length() - 1] != '/')
						path += '/';
					return path;
				};
				string path = getPath();

				while (!this->Import(path)) {
					cout << "Invalid path" << endl;
					path = getPath();
				}
				return 0;
			}
			else {
				this->Create();
				return 1;
			}
		}

		~Restaurant() {
			for (int i = 0; i < employees.size(); i++)
				delete employees[i];
		}
		
	};

	void Start() {
		Restaurant restaurant;
		
		if (!restaurant.CheckDB()) {
			if(restaurant.InsertDB())
				restaurant.Menu(0);
		}
		else {
			if (!restaurant.Import()) {
				cout << "Invalid database" << endl << endl;
				restaurant.InsertDB();
				restaurant.Menu(0);
			}
		}
		system("cls");
		while(true)
			restaurant.Menu(restaurant.Authorization());
	}

}



