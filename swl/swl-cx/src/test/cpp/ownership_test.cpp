//
// Created by Michael Ong on 1/4/20.
//

#include "ownership.hpp"

#include <iostream>
#include <vector>

#include <gtest/gtest.h>

using namespace std;
using namespace swl::cx;

struct TestStruct {

	int value;

	explicit TestStruct(int value): value(value) {
		cout << "Test Struct Allocated." << endl;
	}

	~TestStruct() {
		value = 0;
		cout << "Test Struct Deallocated." << endl;
	};
};

TEST(Ownership, ShouldDestruct) {

	auto pointer = make_own<TestStruct>(100);
	ASSERT_EQ(pointer.isValid(), true);
}

TEST(Ownership, OutsideShouldNotBeValidUponGoingInside) {

	auto pointer = make_own<TestStruct>(100);
	ASSERT_EQ(pointer.isValid(), true);

	{
		auto moved = move(pointer);
		ASSERT_EQ(moved.isValid(), true);
	}

	ASSERT_EQ(pointer.isValid(), false);
}
