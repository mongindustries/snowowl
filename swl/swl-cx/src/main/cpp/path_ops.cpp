//
// Created by micha on 3/29/2020.
//

#include <string>
#include <array>
#include <sstream>
#include <regex>

#include "path_ops.hpp"

using namespace std;

using namespace swl::cx;
using namespace swl::cx_private;

__PathComponents PathOps::components(const string &path, char separator) {

	stringstream stream;

	stream << R"((\w{1,10}\:|\~|\.\.|\.)?((?:\)";
	stream << separator;
	stream << R"({1,2}(?:[\.\-\_\w]+|\.\.|\.))+)(\?(?:(?:[\_\-\w]+\[?\]?)\=\"?[\_\-\w]+\"?\&?)+)?)";

	const string pattern = stream.str();
	regex match(pattern);

	string remaining = path;
	smatch matches;

	array<string, 3> match_items;

	while(regex_search(remaining, matches, match)) {
		match_items[0] = matches.str(1);
		match_items[1] = matches.str(2);
		match_items[2] = matches.str(3);

		remaining = matches.suffix();
	}

	return {
		.protocol = match_items[0],
		.path     = match_items[1],
		.query    = match_items[2]
	};
}

std::regex PathOps::match_path(const string &path, char separator) {

	stringstream ss;

	ss << R"((\)";
	ss << separator;
	ss << R"({1,2}(?:[\.\-\_\w]+|\.\.|\.)))";

	return std::regex(ss.str());
}
