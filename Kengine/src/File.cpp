#include "File.h"

MacroFile File;

void MacroFile::saveMacro()
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ofstream macro;
	macro.open("C:/dev/Kengine/Kengine/res/Test.keg");
	macro << "Writing this to a file.\n";

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].type == INPUT_MOUSE)
		{
			DWORD flags = vec[i].mi.dwFlags;
			flags &= ~(MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE);
			
			switch (flags)
			{
			case MOUSEEVENTF_LEFTDOWN:
				macro << "MOUSEEVENTF_LEFTDOWN ";
				break;
			case MOUSEEVENTF_LEFTUP:
				macro << "MOUSEEVENTF_LEFTUP ";
				break;
			case MOUSEEVENTF_MOVE:
				macro << "MOUSEEVENTF_MOVE ";
				break;
			case MOUSEEVENTF_MIDDLEDOWN:
				macro << "MOUSEEVENTF_MIDDLEDOWN ";
				break;
			case MOUSEEVENTF_MIDDLEUP:
				macro << "MOUSEEVENTF_MIDDLEUP ";
				break;
			case MOUSEEVENTF_WHEEL:
				macro << "MOUSEEVENTF_WHEEL ";
				break;
			case MOUSEEVENTF_HWHEEL:
				macro << "MOUSEEVENTF_HWHEEL ";
				break;
			case MOUSEEVENTF_RIGHTDOWN:
				macro << "MOUSEEVENTF_RIGHTDOWN ";
				break;
			case MOUSEEVENTF_RIGHTUP:
				macro << "MOUSEEVENTF_RIGHTUP ";
				break;
			case MOUSEEVENTF_XDOWN:
				macro << "MOUSEEVENTF_XDOWN ";
				break;
			case MOUSEEVENTF_XUP:
				macro << "MOUSEEVENTF_XUP ";
				break;
			}
			macro << vec[i].mi.dx << " " << vec[i].mi.dy << "\n";
		}
		else if (vec[i].type == INPUT_KEYBOARD)
		{
			
		}
	}

	macro.close();
}