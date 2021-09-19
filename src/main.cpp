#include "MCCScreen.hpp"
#include <chrono>
#include <thread>
int main()
{
	MCCScreen A(400,600,32,"/dev/fb0","/dev/tty1");
	for(int j = 0; j<100;++j){
		A.MCCEraseScreen();
		for(int i = j; i< 100+j; ++i)
		A.putPixel(i,i,255,0,255,122);
		A.MCCUpdateScreen();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
