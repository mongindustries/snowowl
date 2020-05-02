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

	auto pointer = exp::ptr<TestStruct>{ TestStruct(100) };
	ASSERT_EQ(pointer.is_valid(), true);
}

TEST(Ownership, ImplicitCastBool) {

	auto pointer = exp::ptr<TestStruct>(TestStruct { 100 });
	ASSERT_EQ(bool(pointer), true);
}

TEST(Ownership, OutsideShouldNotBeValidUponGoingInside) {

	auto pointer = exp::ptr<TestStruct>(TestStruct { 100 });
	ASSERT_EQ(pointer.is_valid(), true);

	{
		auto moved = std::move(pointer);
		ASSERT_EQ(moved.is_valid(), true);
	}

	ASSERT_EQ(pointer.is_valid(), false);
}
