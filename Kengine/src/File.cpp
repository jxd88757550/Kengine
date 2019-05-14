#include "File.h"

MacroFile File;

const std::map<WORD, std::string> keystr_mapping = {
	{ 0x1B,"ESCAPE" },
	{ 0x70,"F1" },
	{ 0x71,"F2" },
	{ 0x72,"F3" },
	{ 0x73,"F4" },
	{ 0x74,"F5" },
	{ 0x75,"F6" },
	{ 0x76,"F7" },
	{ 0x77,"F8" },
	{ 0x78,"F9" },
	{ 0x79,"F10" },
	{ 0x7A,"F11" },
	{ 0x7B,"F12" },
	{ 0x2C,"PRINT" },
	{ 0x91,"SCROLL" },
	{ 0x13,"PAUSE" },
	{ 0xC0,"BACKQUOTE" },
	{ 0x30,"NUM_0" },
	{ 0x31,"NUM_1" },
	{ 0x32,"NUM_2" },
	{ 0x33,"NUM_3" },
	{ 0x34,"NUM_4" },
	{ 0x35,"NUM_5" },
	{ 0x36,"NUM_6" },
	{ 0x37,"NUM_7" },
	{ 0x38,"NUM_8" },
	{ 0x39,"NUM_9" },
	{ 0xBD,"MINUS" },
	{ 0xBB,"PLUS" },
	{ 0x08,"BACK" },
	{ 0x2D,"INSERT" },
	{ 0x24,"HOME" },
	{ 0X21,"PGUP" },
	{ 0X09,"TAB" },
	{ 0x41,"KEY_A" },
	{ 0x42,"KEY_B" },
	{ 0x43,"KEY_C" },
	{ 0x44,"KEY_D" },
	{ 0x45,"KEY_E" },
	{ 0x46,"KEY_F" },
	{ 0x47,"KEY_G" },
	{ 0x48,"KEY_H" },
	{ 0x49,"KEY_I" },
	{ 0x4a,"KEY_J" },
	{ 0x4b,"KEY_K" },
	{ 0x4c,"KEY_L" },
	{ 0x4d,"KEY_M" },
	{ 0x4e,"KEY_N" },
	{ 0x4f,"KEY_O" },
	{ 0x50,"KEY_P" },
	{ 0x51,"KEY_Q" },
	{ 0x52,"KEY_R" },
	{ 0x53,"KEY_S" },
	{ 0x54,"KEY_T" },
	{ 0x55,"KEY_U" },
	{ 0x56,"KEY_V" },
	{ 0x57,"KEY_W" },
	{ 0x58,"KEY_X" },
	{ 0x59,"KEY_Y" },
	{ 0x5a,"KEY_Z" },
	{ 0xDB,"LEFTBRACE" },
	{ 0xDD,"RIGHTBRACE" },
	{ 0xDC,"BACKSLASH" },
	{ 0x2E,"DEL" },
	{ 0x23,"END" },
	{ 0x22,"PGDN" },
	{ 0x14,"CAPSLOCK" },
	{ 0xBA,"SEMICOLON" },
	{ 0xDE,"SINGLEQUOTE" },
	{ 0XD0,"ENTER" },
	{ 0xA0,"LEFTSHIFT" },
	{ 0xBC,"COMMA" },
	{ 0xBE,"PERIOD" },
	{ 0xBF,"FORWARDSLASH" },
	{ 0xA1,"RIGHTSHIFT" },
	{ 0x26,"UPARROW" },
	{ 0xA2,"LEFTCTRL" },
	{ 0x5B,"WINDOWS" },
	{ 0xA4,"LEFTALT" },
	{ 0x20,"SPACE" },
	{ 0x5D,"MENU" },
	{ 0xA3,"RIGHTCTRL" },
	{ 0x25,"LEFTARROW" },
	{ 0x28,"DOWNARROW" },
	{ 0x27,"RIGHTARROW" },
	{ 0X90,"NUMLOCK" },
	{ 0X6F,"NUMPAD_DIVIDE" },
	{ 0X6A,"NUMPAD_MULTIPLY" },
	{ 0X6B,"NUMPAD_SUBTRACT" },
	{ 0x6B,"NUMPAD_ADD" },
	{ 0xD0,"NUMPAD_ENTER" },
	{ 0x60,"NUMPAD_INSERT" },
	{ 0x6E,"NUMPAD_DELETE" },
	{ 0X60,"NUMPAD_0" },
	{ 0x61,"NUMPAD_1" },
	{ 0x62,"NUMPAD_2" },
	{ 0x63,"NUMPAD_3" },
	{ 0x64,"NUMPAD_4" },
	{ 0x65,"NUMPAD_5" },
	{ 0x66,"NUMPAD_6" },
	{ 0x67,"NUMPAD_7" },
	{ 0x68,"NUMPAD_8" },
	{ 0x69,"NUMPAD_9" },
};

void MacroFile::saveMacroCalls(const std::string & path)
{
	std::vector<INPUT>& vec = KeyLogger::getKeyFrames();

	std::ofstream data;
	data.open(path);

	if (!data.is_open())
	{
		std::cout << "File not saved successfully\n";
		return;
	}
	else
	{
		std::cout << "File saved successfully\n";
	}
		
	data << "void run() {\n";

	for (size_t i = 0; i < vec.size(); ++i) {
		if (vec[i].type == INPUT_MOUSE)
		{
			if (vec[i].mi.time != 0)
			data << "Sleep(" << vec[i].mi.time << ");\n";
			data << "Sender.mouseEvent(MouseCoord(MOUSE_ABSOLUTE, " << 
				vec[i].mi.dx / (0xFFFF / SCREENSIZE_X) << ", " << 
				vec[i].mi.dy / (0xFFFF / SCREENSIZE_Y) << "), ";

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

			auto itr = keystr_mapping.find(vec[i].ki.wVk);

			if(itr == keystr_mapping.end())
				data << "Sender.keyEvent(" << vec[i].ki.wVk << ", " << ((vec[i].ki.dwFlags & KEYEVENTF_KEYUP) ? "true" : "false") << ");\n";
			else
				data << "Sender.keyEvent(" << keystr_mapping.at(vec[i].ki.wVk) << ", " << ((vec[i].ki.dwFlags & KEYEVENTF_KEYUP)? "true" : "false") << ");\n";
		}
	}
	data << "}\n";
	data.close();
}