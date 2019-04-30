#include "File.h"

MacroFile File;

void MacroFile::saveMacro()
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ofstream macro;
	macro.open("Kengine\\res\\Test2.txt");

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].type == INPUT_MOUSE)
		{	
			macro << vec[i].mi.time <<'\n';
			macro << INPUT_MOUSE << '\n';

			macro << vec[i].mi.dx << " " << vec[i].mi.dy << '\n';
			macro << vec[i].mi.dwFlags << '\n';
			macro << vec[i].mi.mouseData << '\n';
		}
		else if (vec[i].type == INPUT_KEYBOARD)
		{
			macro << vec[i].mi.time << '\n';
			macro << INPUT_KEYBOARD << '\n';
			macro << vec[i].ki.wVk << ' ' << vec[i].ki.dwFlags << '\n';
		}
	}

	macro.close();
}