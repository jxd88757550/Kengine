#include <fstream>

#include "KeyLog.h"
#pragma once

class MacroFile;
extern MacroFile File;

class MacroFile {
private:

public:
	void saveMacroData();
	void readMacroData();
};