#include <fstream>

#include "KeyLog.h"
#pragma once

class MacroFile;
extern MacroFile File;

class MacroFile {
private:
	int a;
public:
	void saveMacro();
};