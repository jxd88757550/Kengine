#include <fstream>
#include <map>

#include "KeyLog.h"
#pragma once

class MacroFile;

class MacroFile {
private:

public:
	static void saveMacroCalls(const std::string& path);
};