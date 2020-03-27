//
// Created by Michael Ong on 26/3/20.
//

#include "path.hpp"

#include <cassert>

using namespace std;
using namespace swl::cx;

void path_test() {

	Path samplePath("simple/path/here.txt");

	assert(samplePath.lastComponent == "/here.txt");
	assert(samplePath.extension 		== ".txt");
}
