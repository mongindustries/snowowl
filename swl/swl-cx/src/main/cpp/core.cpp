//
// Created by Michael Ong on 26/3/20.
//

#include "core.hpp"

#include <random>

using namespace std;
using namespace swl::cx;

const char* core::version = "1.0.0";

driver_handle core::make_handle() {

	random_device rd_device;
	default_random_engine engine(rd_device());

	uniform_int_distribution<> distribution(0x1, 0xff'ff);

	const auto one = static_cast<driver_handle>(distribution(engine)) << 0;
	const auto two = static_cast<driver_handle>(distribution(engine)) << 8;
	const auto tri = static_cast<driver_handle>(distribution(engine)) << 16;
	const auto fur = static_cast<driver_handle>(distribution(engine)) << 24;

	return (one | two | tri | fur);
}
