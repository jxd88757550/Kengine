#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include <array>
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

	VOID sendKeys();
};

struct VKKEYINFO {
	WORD vkCode;
	bool isDown;
	DWORD time;
};

class KeyLogger {
private:
	std::vector<VKKEYINFO> keyframes;
	std::array<bool, 255> indices;
public:
	KeyLogger();
	VOID record();
	std::vector<VKKEYINFO>& getKeyFrames();
	std::array<bool, 255>& getIndices();

	#ifdef DEBUG
	VOID printFrames() const;
	#endif
};

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);