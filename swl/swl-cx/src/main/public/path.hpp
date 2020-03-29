//
// Created by Michael Ong on 26/3/20.
//
#pragma once

#include <string>
#include <vector>

#include "headerconv.hpp"

SNOW_OWL_NAMESPACE(cx)

/**
 * Enum for a <code>Path</code>'s type.
 */
enum class PathType {
	/// Denotes that the path is a uniform resource locator.
	url,
	/// Denotes that the path is a location in the file system.
	file,
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
	/// The path extension, if defined.
	std::string extension;
	/// The path's last component.
	std::string lastComponent;
	/// The path type.
	PathType type;

	// properties

	/// Gets path's individual components.
	std::vector<std::string> components() const;

	/// Gets the platform-native path of this <code>Path</code>.
	std::string platformPath() const;

	// modifiers

	void append_path(const Path &toAppend);

	void append_path(const std::string &toAppend, char separator = '/');


	void append_extension(const std::string &extension);

	// initializers

	/**
	 * Creates a <code>Path</code> object. This constructor guesses the path type.
	 * @param [in] absolutePath The absolute path to define this <code>Path</code>.
	 * @param [in] separator The separator style of the path. Defaults to \"/\".
	 */
	Path(const std::string &absolutePath, char separator = '/');

	/**
	 * Creates an appended <code>Path</code> from a source path. This constructor guesses the path type.
	 * @param source The source path that will be the prefix path.
	 * @param append The path to be appended.
	 * @param separator The separator style of the path. Defaults to \"/\".
	 *
	 */
	Path(const Path &source, const std::string& append, char separator = '/');

private:

	// platform-specific

	static char SeparatorStyle;
};

SNOW_OWL_NAMESPACE_END
