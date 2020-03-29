//
// Created by Michael Ong on 26/3/20.
//

#include <gtest/gtest.h>

#include "path.hpp"

using namespace std;
using namespace swl::cx;

TEST(PathTest, ShouldConstruct) {

	Path samplePath("/absolute/path");

	ASSERT_EQ   (samplePath.absolutePath, "C:/absolute/path");
	ASSERT_EQ   (samplePath.lastComponent(), "/path");
}

TEST(PathTest, ShouldConstrucWindowsStyle) {

	Path samplePath("C:/absolute/path");

	ASSERT_EQ   (samplePath.absolutePath, "C:/absolute/path");
	ASSERT_EQ   (samplePath.lastComponent(), "/path");
}

TEST(PathTest, ShouldAppend) {

	Path samplePath("/root");

	ASSERT_EQ(samplePath.absolutePath, "/root");

	samplePath.appendPath("subroot");

	ASSERT_EQ(samplePath.absolutePath, "/root/subroot");
}

TEST(PathTest, ShouldAppendPaths) {

	Path samplePath("/root");

	ASSERT_EQ(samplePath.absolutePath, "/root");

	samplePath.appendPath(Path("another/subpath"));

	ASSERT_EQ(samplePath.absolutePath, "/root/another/subpath");
}

TEST(PathTest, ShouldAcceptCustomSeparatorStyle) {

	Path path(":deps:googletest", ':');

	ASSERT_EQ(path.absolutePath, ":deps:googletest");
	ASSERT_EQ(path.lastComponent(), ":googletest");
}

TEST(PathTest, ShouldAcceptHttpPath) {

	Path path("http://www.google.com?userauth=0");

	ASSERT_EQ(path.absolutePath, "https://www.google.com?userauth=0");
}

TEST(PathTest, ShouldMatchPlatformPath) {

	Path path("/test/path/over/here.txt");

#if defined(SWL_WIN32)
	ASSERT_EQ(path.platformPath(), "\\test\\path\\over\\here.txt");
#else
	ASSERT_EQ(path.platformPath(), "");
#endif
}
