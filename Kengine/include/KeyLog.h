#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <stdio.h>
#include <chrono>
#include <ctime>

class KeyLogger;
class KeySender;
extern KeyLogger Logger;
extern KeySender Sender;

#pragma once

class KeySender {
private:
	INPUT ip;
public:
	KeySender();
	VOID keyDown(WORD vkCode);
	VOID keyUp(WORD vkCode);
	VOID KeyTest(WORD vkCode);

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
	VOID record();
	std::vector<INPUT>& getKeyFrames();

	#ifdef DEBUG
	VOID printFrames() const;
	#endif
};

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);