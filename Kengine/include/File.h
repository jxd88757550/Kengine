#include <fstream>

#include "KeyLog.h"
#include <map>
#pragma once

class MacroFile;
extern MacroFile File;

class MacroFile {
private:

public:
	void saveMacroCalls(const std::string& path);
};