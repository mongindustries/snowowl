//
// Created by Michael Ong on 26/3/20.
//

#include "core.hpp"

#include <random>

using namespace std;
using namespace swl::cx;

const char* Core::version = "1.0.0";

DriverHandle Core::makeHandle() {

	random_device rd_device;
	default_random_engine engine(rd_device());

	uniform_int_distribution<int> distribution(0, 0xff'ff'ff'ff);

	const auto one = static_cast<DriverHandle>(distribution(engine)) << 0;
	const auto two = static_cast<DriverHandle>(distribution(engine)) << 8;
	const auto tri = static_cast<DriverHandle>(distribution(engine)) << 16;
	const auto fur = static_cast<DriverHandle>(distribution(engine)) << 24;

	return (one | two | tri | fur);
}
