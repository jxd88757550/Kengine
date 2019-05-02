#include <fstream>

#include "KeyLog.h"
#pragma once

class MacroFile;
extern MacroFile File;

class MacroFile {
private:

public:
	void saveMacroData(const std::string& path);
	void readMacroData(const std::string& path);

	void saveMacroCalls(const std::string& path);
};