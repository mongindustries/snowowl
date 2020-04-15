//
// Created by Michael Ong on 13/4/20.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

#include <string>

#include "file_manager.hpp"

using namespace std;
using namespace filesystem;

using namespace swl::cx;

string resourcePathFromBundle() {
	auto cstr = [[NSBundle mainBundle].resourceURL.path cStringUsingEncoding:NSUTF8StringEncoding];
	return string(cstr);
}

path FileManager::resourcePath = resourcePathFromBundle();
