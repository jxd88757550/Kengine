#include "File.h"

MacroFile File;

void MacroFile::saveMacroData()
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ofstream macro;
	macro.open("Kengine\\res\\Test2.txt");

	macro << vec.size() <<'\n';

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].type == INPUT_MOUSE)
		{
			macro << INPUT_MOUSE << '\n';
			macro << vec[i].mi.time <<'\n';
			macro << vec[i].mi.dx << " " << vec[i].mi.dy << '\n';
			macro << vec[i].mi.dwFlags << '\n';
			macro << vec[i].mi.mouseData << '\n';
		}
		else if (vec[i].type == INPUT_KEYBOARD)
		{
			macro << INPUT_KEYBOARD << '\n';
			macro << vec[i].ki.time << '\n';
			macro << vec[i].ki.wVk << ' ' << vec[i].ki.dwFlags << '\n';
		}
	}

	macro.close();
}

void MacroFile::readMacroData()
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ifstream data;
	data.open("Kengine\\res\\Test2.txt");
	
	size_t vecsize;

	data >> vecsize;
	vec.resize(vecsize);

	for (size_t i = 0; i < vec.size(); ++i) {
		ZeroMemory(&vec[i], sizeof(vec[i]));

		data >> vec[i].type;
		if (vec[i].type == INPUT_KEYBOARD) {
			data >> vec[i].ki.time >> vec[i].ki.wVk >> vec[i].ki.dwFlags;
		}
		else if (vec[i].type == INPUT_MOUSE) {
			data >> vec[i].mi.time >> vec[i].mi.dx >> vec[i].mi.dy >> vec[i].mi.dwFlags >> vec[i].mi.mouseData;
		}
	}

	data.close();
}