//
// Created by Michael Ong on 26/3/20.
//

#include <gtest/gtest.h>

#include "path.hpp"

using namespace std;
using namespace swl::cx;

TEST(PathTest, ShouldConstruct) {

	Path samplePath("absolute/path");

	ASSERT_EQ   (samplePath.absolutePath, "absolute/path");
	ASSERT_EQ   (samplePath.lastComponent, "path");
	ASSERT_EQ   (samplePath.type, PathType::file);
}

TEST(PathTest, ShouldAppend) {

	Path samplePath("root");

	ASSERT_EQ(samplePath.absolutePath, "root");

	samplePath.append_path("subroot");

	ASSERT_EQ(samplePath.absolutePath, "root/subroot");
}

TEST(PathTest, ShouldAppendPaths) {

	Path samplePath("root");

	ASSERT_EQ(samplePath.absolutePath, "root");

	samplePath.append_path(Path("another/subpath"));

	ASSERT_EQ(samplePath.absolutePath, "root/another/subpath");
}

TEST(PathTest, ShouldAcceptCustomSeparatorStyle) {

	Path path(":deps:googletest", ':');

	ASSERT_EQ(path.absolutePath, ":deps/googletest");
	ASSERT_EQ(path.lastComponent, "googletest");
}

TEST(PathTest, ShouldBeOfTypeURL) {

	Path path("http://www.google.com");

	ASSERT_EQ(path.absolutePath, "https://www.google.com");
	ASSERT_EQ(path.type, PathType::url);
}
