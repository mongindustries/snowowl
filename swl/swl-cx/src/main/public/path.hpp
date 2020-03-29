//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include <string>

#include <map>
#include <vector>

#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

struct PathError: public std::exception {

	PathError(const std::string &why): exception(why.c_str()) {
	}
};

/***
 * The components of a <code>Path</code> object.
 */
struct PathComponents {
	/// The protocol used. Can be blank. (eg: http: of http://google.com)
	std::string protocol;
	/// The full locator path. (eg: /usr/bin/gradle of /usr/bin/gradle)
	std::string fullPath;

	/// Individualized components of <code>fullPath</code>. (eg: [ "/usr", "/bin", "/gradle" ] of /usr/bin/gradle ])
	std::vector<std::string> pathItems;

	/// Full query path. (eg: ?authuser=1 of https://google.com?authuser=1)
	std::string fullQuery;
	/// Individualized components of <code>fullQuery</code>. (eg: { "authuser" : [ "1" ] } of https://google.com?authuser=1)
	std::map<std::string, std::vector<std::string>> query;
};

/**
 * Encapsulates a file path. It provides basic services like querying the components, extension, and relativity.
 *
 * @example
 *
 * This creates an absolute path "Absolute/Path/To/Somewhere.There":
 *
 * @code
 * Path path("/Absolute/Path/To/Somewhere.There");
 * @endcode
 *
 * This assumes a specific kind of separator style (Gradle project style):
 *
 * @code
 * Path path("Shared:SharedProject", ':');
 * @endcode
 *
 */
struct Path {

	// attributes

	/// The absolute location of the path (as it was created in the initializer).
	std::string absolutePath;

	// properties

	/// Gets path's individual components.
	[[nodiscard]] PathComponents components() const;

	/// Gets the platform-native path.
	[[nodiscard]] std::string platformPath() const;

	/// Gets the last component.
	[[nodiscard]] std::string lastComponent() const;

	/// Normalizes the absolute path.
	/// @example
	/// @code
	/// Path something(":deps:googletest");
	/// Path normalized = something.normalized(); // returns "/deps/googletest"
	[[nodiscard]] Path normalized() const;

	// modifiers

	void appendPath(const Path &toAppend);

	void appendPath(const std::string &toAppend);


	void appendExtension(const std::string &extension);

	// initializers

	/**
	 * Creates a <code>Path</code> object. This constructor guesses the path type.
	 * @param [in] absolutePath The absolute path to define this <code>Path</code>.
	 * @param [in] separator The separator style of the path. Defaults to \"/\".
	 */
	Path(std::string absolutePath, char separator = '/');

private:

	char separator;

	// platform-specific

	static char SeparatorStyle;

	static std::string GetPlatformPath(const Path *path);
};

SNOW_OWL_NAMESPACE_END
