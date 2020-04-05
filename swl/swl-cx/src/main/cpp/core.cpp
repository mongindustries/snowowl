//
// Created by Michael Ong on 26/3/20.
//

#include "core.hpp"

#include <random>

using namespace std;
using namespace swl::cx;

string Core::version = "1.0.0";

unsigned long Core::makeHandle() {

	random_device rd_device;
	default_random_engine engine(rd_device());

	uniform_int_distribution<char> distribution((char)0, (char)0xff);

	auto one = (unsigned long)  distribution(engine);
	auto two = (unsigned long) (distribution(engine) << 8 );
	auto tri = (unsigned long) (distribution(engine) << 16);
	auto fur = (unsigned long) (distribution(engine) << 24);

	return (one | two | tri | fur);
}
