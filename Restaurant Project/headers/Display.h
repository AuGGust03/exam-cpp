#pragma once
#include <iostream>

#include <conio.h>
#include "libraries.h"

using namespace std;




namespace display {
	int PrintMenu(const vector<string>rows, string title = string()) {

		int c;
		int i = 0;
		int size = rows.size();


		do {
			system("cls");

			if (title != string()) {
				cout << title << endl << endl;
			}

			string menu;
			for (int j = 0; j < size; j++) {
				if (j == i)
					menu += "->";
				menu += "\t" + rows[j] + '\n';
			}

			cout << menu;

			do {
				c = _getch();
			} while (c != 224 && c != 13);

			if (c == 224) {
				c = _getch();
				if (c == 72)
					i > 0 ? i-- : i = size - 1;
				else if (c == 80)
					i < size - 1 ? i++ : i = 0;
			}

		} while (c != 13);

		system("cls");
		return i;
	}

	string Pass(string out) {
		cout << out;
		string password;

		char c = 'a';
		do {
			c = _getch();
			if (int(c) == 13)
				continue;
			if (int(c) == 220) {
				c = _getch();
				continue;
			}
			if (int(c) == 8) {
				if (c == 8 && password.size() != 0) {
					cout << '\b' << ' ' << '\b';
					password.pop_back();
				}
				continue;
			}
			password += c;
			cout << "*";
		} while (c != 13);
		return password;
	}

	template<class T>
	T GetData(string out) {
		T data;
		cout << out;

		string type = typeid(data).name();
		if (type == "int") {
			string temp;
			try {
				cin >> temp;
				data = stoi(temp);
			}
			catch (std::invalid_argument&) {
				cout << "Invalid input, try again" << endl;
				data = GetData<int>(out);
			}
		}
		else {
			cin >> data;
		}
		return data;


	}

	void Print(const vector<string>& rows, string title = string()) {
		if (title != string()) {
			cout << title << endl << endl;
		}
		for (int i = 0; i < rows.size(); i++)
			cout << "\t" << rows[i] << endl;
	}

	template<class T>
	bool IsDigit(T a) {
		for (int i = 0; i < a.length(); i++) {

		}
	}


}
