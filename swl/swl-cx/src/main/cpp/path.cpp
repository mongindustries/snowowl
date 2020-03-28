//
// Created by Michael Ong on 26/3/20.
//
#include "path.hpp"

#include <regex>
#include <sstream>
#include <functional>

using namespace std;
using namespace swl::cx;

vector<string>
	componetizePath(const string &input, char separator) {

		try {

			stringstream pattern;
			pattern << R"(((?:[^\)" << separator << R"(].)+)\)" << separator << "*";

			string       remain = input;

			smatch       results;
			regex        toMatch(pattern.str());

			vector<string> components;

			while (regex_search(remain, results, toMatch)) {

				vector<string> items(results.begin(), results.end());

				components.push_back(items.back());

				remain = results.suffix();
			}

			return components;
		} catch (const regex_error &regexError) {
			return {};
		}
	}

string
	normalizePath(const vector<string> &paths) {

		stringstream result;

		for (const auto &item : paths) {

			result << item;

			if (item != paths.back()) {
				result << '/';
			}
		}

		return result.str();
	}

vector<string>
	appendPath(const vector<string> &source, const vector<string> &append) {

		vector<string> output(source);

		for (const auto &item : append) {
			output.push_back(item);
		}

		return output;
	}


Path::Path
	(const string &absolutePath, char separator):
	absolutePath(normalizePath(componetizePath(absolutePath, separator))),
	extension(),
	lastComponent(componetizePath(absolutePath, separator).back()),
	type(PathType::file) {

	}

Path::Path
	(const Path &source, const string& append, char separator):
	absolutePath(normalizePath(appendPath(componetizePath(source.absolutePath, separator), componetizePath(append, separator)))),
	extension(),
	lastComponent(componetizePath(append, separator).back()),
	type(PathType::file) {

	}


vector<string>
	Path::components() const {
		return componetizePath(absolutePath, '/');
	}

string
	Path::platformPath() const {
	}


void
	Path::append_path(const Path &toAppend) {

		const auto toAdd = toAppend.components();
		const auto pFrom = componetizePath(absolutePath, '/');

		absolutePath  = normalizePath(appendPath(toAdd, pFrom));
		lastComponent = toAdd.back();
	}

void
	Path::append_path(const std::string &toAppend) {
	}


void
	Path::append_extension(const std::string &extension) {
	}
