#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <memory>
#include <fstream>
#include <filesystem>
#include <typeinfo>

using namespace std;


vector<string> Split(string str, char sep = ' ') {
	vector<string> words;
	string word = string();
	int length = str.length();
	for (int i = 0; i <= length; i++) {
		if (str[i] == sep || str[i] == '\0') {
			words.push_back(word);
			word.clear();
		}
		else {
			word += str[i];
		}
	}
	return words;
}

string Join(vector<string> words, string sep = " ") {
	string str = string();
	int size = words.size();
	for (int i = 0; i < size - 1; i++) {
		str += words[i] + sep;
	}
	str += words[size - 1];
	return str;
}

auto min(vector<int>& ints) {
	if (ints.size() == 0)
		throw "Size = 0";
	else if (ints.size() == 1)
		return ints.begin();
	else {
		auto res = ints.begin();
		for (auto it = ints.begin() + 1; it != ints.end(); it++) {
			res = *res < *it ? res : it;
		}
		return res;
	}

}

