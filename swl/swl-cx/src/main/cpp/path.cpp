//
// Created by Michael Ong on 26/3/20.
//
#include "path.hpp"

#include <regex>
#include <functional>
#include <utility>

#include "path_ops.hpp"

using namespace std;
using namespace swl::cx;
using namespace swl::cx_private;

Path::Path
	(string absolutePath, char separator): absolutePath(std::move(absolutePath)), separator(separator) {

		try {
			(void) PathOps::components(this->absolutePath, separator);
		} catch (const PathError &error) {
			throw error;
		}
	}


PathComponents
	Path::components() const {

		auto cpx = PathOps::components(absolutePath, separator);

		// match individual path:
		vector<string> pathItems; {

			smatch match_items;
			const auto match = PathOps::match_path(absolutePath, separator);

			string remaining = cpx.path;

			while (regex_search(remaining, match_items, match)) {
				pathItems.push_back(match_items.str(0));
				remaining = match_items.suffix();
			}
		}

		return {
			.protocol   = cpx.protocol,

			.fullPath   = cpx.path,
			.pathItems  = pathItems,

			.fullQuery  = cpx.query,
		};
	}

std::string
	Path::lastComponent() const {
		return components().pathItems.back();
	}

string
	Path::platformPath() const {
		return GetPlatformPath(this);
	}

Path
	Path::normalized() const {

	}

void
	Path::appendPath(const Path &toAppend) {
		if (toAppend.separator != separator) {
			// normalize paths first before appending
		} else {

		}
	}

void
	Path::appendPath(const std::string &toAppend) {
	}


void
	Path::appendExtension(const std::string &extension) {
	}
