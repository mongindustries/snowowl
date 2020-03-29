//
// Created by Michael Ong on 26/3/20.
//
#include <sstream>

#include "path.hpp"

using namespace std;
using namespace swl::cx;

char Path::SeparatorStyle = '\\';

string Path::GetPlatformPath(const Path *path) {

	return "";

//	auto         components = path->components();
//	stringstream output;
//
//	for (const auto &item : components) {
//		output << item;
//
//		if (item != components.back()) {
//			output << SeparatorStyle;
//		}
//	}
//
//	return output.str();
}
