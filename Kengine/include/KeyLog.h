#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <bitset>

class KeyLogger;
class EventSender;
extern KeyLogger Logger;
extern EventSender Sender;

#pragma once

enum TYPE : WORD {
	MOUSE_IGNORE,
	MOUSE_ABSOLUTE,
	MOUSE_OFFSET,
	MOUSE_RELATIVE
};

struct MouseCoord {
	LONG x;
	LONG y;
	TYPE type;

	MouseCoord(TYPE type=MOUSE_IGNORE, LONG x=0, LONG y=0);
};

enum MouseEvent : WORD
{
	MOVE,
	HWHEEL_RIGHT,
	HWHEEL_LEFT,
	WHEEL_DOWN,
	WHEEL_UP,
	LEFT_CLICK,
	LEFT_DOWN,
	LEFT_UP,
	RIGHT_CLICK,
	RIGHT_DOWN,
	RIGHT_UP,
	MIDDLE_CLICK,
	MIDDLE_DOWN,
	MIDDLE_UP,
	X1_DOWN,
	X1_UP,
	X2_DOWN,
	X2_UP,
};

enum KeyEvent : WORD
{
	ESCAPE = 0x1B,
	F1 = 0x70,
	F2 = 0x71,
	F3 = 0x72,
	F4 = 0x73,
	F5 = 0x74,
	F6 = 0x75,
	F7 = 0x76,
	F8 = 0x77,
	F9 = 0x78,
	F10 = 0x79,
	F11 = 0x7A,
	F12 = 0x7B,
	PRINT = 0x2C,
	SCROLL = 0x91,
	PAUSE = 0x13,
	BACKQUOTE = 0xC0,
	NUM_0 = 0x30,
	NUM_1 = 0x31,
	NUM_2 = 0x32,
	NUM_3 = 0x33,
	NUM_4 = 0x34,
	NUM_5 = 0x35,
	NUM_6 = 0x36,
	NUM_7 = 0x37,
	NUM_8 = 0x38,
	NUM_9 = 0x39,
	MINUS = 0xBD,
	PLUS = 0xBB,
	BACK = 0x08,
	INSERT = 0x2D,
	HOME = 0x24,
	PGUP = 0X21,
	TAB = 0X09,
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4a,
	KEY_K = 0x4b,
	KEY_L = 0x4c,
	KEY_M = 0x4d,
	KEY_N = 0x4e,
	KEY_O = 0x4f,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5a,
	LEFTBRACE = 0xDB,
	RIGHTBRACE = 0xDD,
	BACKSLASH = 0xDC,
	DEL = 0x2E,
	END = 0x23,
	PGDN = 0x22,
	CAPSLOCK = 0x14,
	SEMICOLON = 0xBA,
	SINGLEQUOTE = 0xDE,
	ENTER = 0XD0,
	LEFTSHIFT = 0xA0,
	COMMA = 0xBC,
	PERIOD = 0xBE,
	FORWARDSLASH = 0xBF,
	RIGHTSHIFT = 0xA1,
	UPARROW = 0x26,
	LEFTCTRL = 0xA2,
	WINDOWS = 0x5B,
	LEFTALT = 0xA4,
	SPACE = 0x20,
	MENU = 0x5D,
	RIGHTCTRL = 0xA3,
	LEFTARROW = 0x25, 
	DOWNARROW = 0x28,
	RIGHTARROW = 0x27,

	NUMLOCK = 0X90,
	NUMPAD_DIVIDE = 0X6F,
	NUMPAD_MULTIPLY = 0X6A,
	NUMPAD_SUBTRACT = 0X6B,
	NUMPAD_ADD = 0x6B,
	NUMPAD_ENTER = 0xD0,
	NUMPAD_INSERT = 0x60,
	NUMPAD_DELETE = 0x6E,
	NUMPAD_0 = 0X60,
	NUMPAD_1 = 0x61,
	NUMPAD_2 = 0x62,
	NUMPAD_3 = 0x63,
	NUMPAD_4 = 0x64,
	NUMPAD_5 = 0x65,
	NUMPAD_6 = 0x66,
	NUMPAD_7 = 0x67,
	NUMPAD_8 = 0x68,
	NUMPAD_9 = 0x69,
};

class EventSender {
public:
	VOID mouseEvent(const MouseCoord& coords, enum MouseEvent e);
	VOID keyEvent(WORD vkCode, bool isUp);
	VOID sendKeys();
};

class KeyLogger {
private:
	std::vector<INPUT> keyframes;
public:
	std::array<std::string, 255> codes=
	{
#include "VKCodes.h"	
	};
	std::pair<int, std::string> d;
	
	VOID record();
	std::vector<INPUT>& getKeyFrames();

	#ifdef DEBUG
	VOID printFrames() const;
	#endif
};

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);