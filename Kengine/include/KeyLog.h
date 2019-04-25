#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include <array>
#include <stdio.h>

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
	std::array<WORD, sizeof(WORD)> indices;
public:
	VOID record();
	std::vector<VKKEYINFO>& getKeyFrames();
	std::array<WORD, sizeof(WORD)> getIndices();

	#ifdef DEBUG
	VOID printFrames() const;
	#endif
};

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);