#include "File.h"

MacroFile File;

void MacroFile::saveMacroData(const std::string& path)
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ofstream data;
	data.open(path);

	data << vec.size() <<'\n';

	for (size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i].type == INPUT_MOUSE)
		{
			data << INPUT_MOUSE << '\n';
			data << vec[i].mi.time <<'\n';
			data << vec[i].mi.dx << " " << vec[i].mi.dy << '\n';
			data << vec[i].mi.dwFlags << '\n';
			data << vec[i].mi.mouseData << '\n';
		}
		else if (vec[i].type == INPUT_KEYBOARD)
		{
			data << INPUT_KEYBOARD << '\n';
			data << vec[i].ki.time << '\n';
			data << vec[i].ki.wVk << ' ' << vec[i].ki.dwFlags << '\n';
		}
	}

	data.close();
}

void MacroFile::readMacroData(const std::string& path)
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ifstream data;
	data.open(path);
	
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

void MacroFile::saveMacroCalls(const std::string & path)
{
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	std::ofstream data;
	std::cout << path;
	data.open(path);

	if (!data.is_open()) {
		std::cout << "Failed file\n";
		return;
	}

	data << "void run() {\n";

	for (size_t i = 0; i < vec.size(); ++i) {
		if (vec[i].type == INPUT_MOUSE)
		{
			if (vec[i].mi.time != 0)
			data << "Sleep(" << vec[i].mi.time << ");\n";
			data << "Sender.mouseEvent(MouseCoord(MOUSE_ABSOLUTE, " << 
				vec[i].mi.dx / (0xFFFF / GetSystemMetrics(SM_CXSCREEN)) << ", " << 
				vec[i].mi.dy / (0xFFFF / GetSystemMetrics(SM_CYSCREEN)) << "), MouseEvent::";

			DWORD flags = vec[i].mi.dwFlags;
			flags &= ~(MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE);

			switch (flags)
			{
			case MOUSEEVENTF_LEFTDOWN:
				data << "LEFT_DOWN";
				break;
			case MOUSEEVENTF_LEFTUP:
				data << "LEFT_UP";
				break;
			case MOUSEEVENTF_MOVE:
				data << "MOVE";
				break;
			case MOUSEEVENTF_MIDDLEDOWN:
				data << "MIDDLE_DOWN";
				break;
			case MOUSEEVENTF_MIDDLEUP:
				data << "MIDDLE_UP";
				break;
			case MOUSEEVENTF_WHEEL:
				if (vec[i].mi.mouseData < 0)
					data << "WHEEL_DOWN";
				else
					data << "WHEEL_UP";
				break;
			case MOUSEEVENTF_HWHEEL:
				if (vec[i].mi.mouseData < 0)
					data << "HWHEEL_RIGHT";
				else
					data << "HWHEEL_LEFT";
				//horizontal wheel not working atm
				break;
			case MOUSEEVENTF_RIGHTDOWN:
				data << "RIGHT_DOWN";
				break;
			case MOUSEEVENTF_RIGHTUP:
				data << "RIGHT_UP";
				break;
			case MOUSEEVENTF_XDOWN:
				if (vec[i].mi.mouseData & XBUTTON1)
					data << "X1_DOWN";
				else
					data << "X2_DOWN";
				break;
			case MOUSEEVENTF_XUP:
				if (vec[i].mi.mouseData & XBUTTON1)
					data << "X1_UP";
				else
					data << "X2_UP";
				break;
			}
			data << ");\n";
		}
		else if (vec[i].type == INPUT_KEYBOARD)
		{
			if(vec[i].ki.time != 0)
			data << "Sleep(" << vec[i].ki.time << ");\n";
			data << "Sender.keyEvent(" << vec[i].ki.wVk << ", " << ((vec[i].ki.dwFlags & KEYEVENTF_KEYUP)? "true" : "false") << ");\n";
		}
	}
	data << "}\n";
	data.close();
}