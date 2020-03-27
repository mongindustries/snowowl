//
// Created by Michael Ong on 26/3/20.
//
#include "path.hpp"

#include <regex>
#include <iostream>

using namespace std;
using namespace swl::cx;

string getLastComponent(std::string &check) {
	smatch results;
	regex  toMatch(R"(/[\.\-\w]+\.[\w]*$)");

	string result;

	if (regex_search(check, results, toMatch)) {
		for (auto &item : results) {
			result = item.str();
		}
	}

	return result;
}

Path::Path(std::string &absolutePath):
	absolutePath(absolutePath),
	lastComponent(getLastComponent(absolutePath)) {

}

Path::Path(std::string absolutePath):
		absolutePath(absolutePath),
		lastComponent(getLastComponent(absolutePath)) {

}
