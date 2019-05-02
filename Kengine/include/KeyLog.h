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

struct MouseCoord {
	LONG x;
	LONG y;
	char type;

	enum TYPE : unsigned int {
		//ABSOLUTE,
		//RELATIVE,
		//IGNORE,
		OFFSET
	};

	MouseCoord(char type=0, LONG x=0, LONG y=0);
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