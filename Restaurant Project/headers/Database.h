#pragma once
#include "libraries.h"
#include <filesystem>

class DB {
private:
	const string path = "database/";

	void operator=(const DB&) {}

	void ClearFile(string path) {
		ofstream fout(path);
		fout.close();
	}
public:
	DB() {}

	string GetPath() const { return path; }

	void Create() {
		if (!filesystem::exists(path))
			filesystem::create_directory(path);
		ClearFile(path + "employees.txt");
	}

	bool Import(string path) {
		if (!Check(path))
			return 0;
		if (!filesystem::exists(this->path))
			filesystem::create_directory(this->path);

		string file = "employees.txt";
		CopyFile(path + file, this->path + file);

		file = "discounts.txt";
		CopyFile(path + file, this->path + file);

		file = "tables.txt";
		CopyFile(path + file, this->path + file);

		return 1;
	}

	static void ToFile(string path, string str, bool app = 1) {
		ofstream fout;
		if (app)
			fout.open(path, ios::app);
		else
			fout.open(path);

		fout << str;

		fout.close();

	}

	void CopyFile(string from, string to) {
		ifstream fin(from);
		ofstream fout(to);

		bool c1 = !fin.is_open();
		bool c2 = !fout.is_open();

		if (!fin.is_open() || !fout.is_open())
			return;

		fout << fin.rdbuf();
	}

	bool Check(string path = "database/") const {
		ifstream fin(path + "employees.txt");

		if (!fin.is_open() || fin.peek() == EOF) {
			fin.close();
			return 0;
		}

		string s;
		getline(fin, s);
		vector<string> words = Split(s);

		if (words.size() != 5)
			return 0;

		if (words[0] != "0" || words[1] != "Director")
			return 0;

		fin.close();
		return 1;
	}


	void AddEmpBD(string data, int index = 0) {
		if (index == 0) {
			if (!LineBreak(path + "employees.txt"))
				data.insert(data.begin(), '\n');
			ToFile(path + "employees.txt", data);
		}
		else
			InsertToFile(path + "employees.txt", data, index);
	}

	void RemoveEmpBD(int index) {
		RemoveStr(path + "employees.txt", index);
	}

	void EditEmpBD(int index, string type, string typeData) {
		int typeNum;
		if (type == "Password")
			typeNum = 2;
		else if (type == "Name")
			typeNum = 3;
		else
			typeNum = 4;

		EditStr(path + "employees.txt", index, typeNum, typeData);
	}

	void RemoveStr(string FilePath, int index) {
		vector<string> data = ReadVectorData(FilePath);
		data.erase(data.begin() + index);
		WriteVectorData(data, FilePath);
	}

	void InsertToFile(string FilePath, string row, int index) {
		vector<string> data = ReadVectorData(FilePath);
		data.insert(data.begin() + index, row);
		WriteVectorData(data, FilePath);
	}

	void EditStr(string FilePath, int index, int type, string typeData) {
		vector<string> data = ReadVectorData(FilePath);
		vector<string> strData = Split(data[index]);
		strData[type] = typeData;
		data[index] = Join(strData);
		WriteVectorData(data, FilePath);
	}

	vector<string> ReadVectorData(string FilePath) {
		vector<string> data;
		string str;
		ifstream fin(FilePath);
		while (!fin.eof()) {
			getline(fin, str);
			if (str == "\n" || str == "")
				continue;
			data.push_back(str);
		}
		fin.close();
		return data;
	}

	void WriteVectorData(vector<string>& data, string FilePath) {
		ofstream fout(FilePath);
		for (int i = 0; i < data.size(); i++) {
			if (data[i] == "\n" || data[i] == "")
				continue;
			fout << data[i];
			if (data[i][data[i].size() - 1] != '\n')
				fout << endl;
		}
		fout << endl;
		fout.close();
	}

	bool LineBreak(string FilePath) {
		ifstream fin(FilePath);
		string str;
		getline(fin, str);
		while (!fin.eof())
			getline(fin, str);
		fin.close();
		if (str == "\n" || str == "")
			return 1;
		return 0;
	}

	void AddDiscount(int key, int value) {
		string FilePath = path + "discounts.txt";
		string data = to_string(key) + " " + to_string(value);
		if (!LineBreak(path + "employees.txt"))
			data.insert(data.begin(), '\n');
		ToFile(FilePath, data);
	}

	void RemoveDiscountDB(int key) {
		string FilePath = path + "discounts.txt";
		int index = FindDiscount(key);
		RemoveStr(FilePath, index);
	}

	void EditDiscountDB(int key, int newValue) {
		string FilePath = path + "discounts.txt";
		int index = FindDiscount(key);
		if (index == -1)
			return;
		EditStr(FilePath, index, 1, to_string(newValue));
	}

	int FindDiscount(int key) {
		string FilePath = path + "discounts.txt";
		int index = -1;
		ifstream fin(FilePath);
		vector<string> data = ReadVectorData(FilePath);
		for (int i = 0; i < data.size(); i++) {
			string row;
			vector<string> rows;
			getline(fin, row);
			if (data[i] == "\n" || data[i] == "")
				continue;
			rows = Split(row);
			row = rows[0];
			if (row == to_string(key)) {
				index = i;
				break;
			}
		}
		return index;
	}

	void AddTable(int id) {
		string FilePath = path + "tables.txt";
		string data = to_string(id) + " 1";
		if (!LineBreak(FilePath))
			data.insert(data.begin(), '\n');
		ToFile(FilePath, data);
	}

	void RemoveTable() {
		string FilePath = path + "tables.txt";
		vector<string> data = ReadVectorData(FilePath);
		data.erase(data.begin() + (data.size() - 1));
		WriteVectorData(data, FilePath);
	}

	void AddDish(string name, int price) {
		string FilePath = path + "menu.txt";
		string data = name + " " + to_string(price);
		if (!LineBreak(FilePath))
			data.insert(data.begin(), '\n');
		ToFile(FilePath, data);
	}

	void RemoveDish(string name) {
		string FilePath = path + "menu.txt";
		vector<string> data = ReadVectorData(FilePath);
		vector<string> dataRow;
		int i = 0;
		for (i; i < data.size(); i++) {
			dataRow = Split(data[i]);
			if (dataRow[0] == name)
				break;
		}
		data.erase(data.begin() + i);
		WriteVectorData(data, FilePath);
	}

	void ChangeDish(string name, int newPrice) {
		string FilePath = path + "menu.txt";
		vector<string> data = ReadVectorData(FilePath);
		vector<string> dataRow;
		int i = 0;
		for (i; i < data.size(); i++) {
			dataRow = Split(data[i]);
			if (dataRow[0] == name)
				break;
		}
		dataRow[1] = to_string(newPrice);
		data[i] = Join(dataRow);
		WriteVectorData(data, FilePath);
	}

	void RemoveOrder(int id) {
		string FilePath = path + "tables.txt";
		vector<string> data = ReadVectorData(FilePath);
		vector<string> dataRow;
		int i = 0;
		for (i; i < data.size(); i++) {
			dataRow = Split(data[i]);
			if (dataRow[0] == to_string(id))
				break;
		}
		data[i].clear();
		data[i] = dataRow[0] + " " + dataRow[1] + " " + dataRow[2];
		WriteVectorData(data, FilePath);
	}

	void AddOrder(int id, vector<string> newData) {
		string FilePath = path + "tables.txt";
		vector<string> data = ReadVectorData(FilePath);
		vector<string> dataRow;
		int i = 0;
		for (i; i < data.size(); i++) {
			dataRow = Split(data[i]);
			if (dataRow[0] == to_string(id))
				break;
		}
		for (int i = 0; i < newData.size(); i++)
			dataRow.push_back(newData[i]);
		data[i] = Join(dataRow);
		WriteVectorData(data, FilePath);
	}

	void EmptyTable(int id, int free = 1, string name = string()) {
		string FilePath = path + "tables.txt";
		vector<string> data = ReadVectorData(FilePath);
		vector<string> dataRow;
		int i = 0;
		for (i; i < data.size(); i++) {
			dataRow = Split(data[i]);
			if (dataRow[0] == to_string(id))
				break;
		}
		data[i] = dataRow[0] + " " + to_string(free);
		if (name != string())
			data[i] += " " + name;
		WriteVectorData(data, FilePath);
	}

};

static DB db;
static map<int, int> discounts;
static map<string, int> menu;