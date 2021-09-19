#include "MCCScreen.hpp"
#include <iostream>
#include <chrono>
#include <thread>
int main()
{
	MCCScreen A;
	int counter = 0;
	auto currentimer = std::chrono::system_clock::now();
	std::cout << (int)A.init(400,600,32,"/dev/fb0","/dev/tty1");
	for(int j = 0; j<1000;++j){
		A.eraseScreen();
		for(int i = j%100; i< (100+j)%200; ++i)
		A.putPixel(i,i,255,0,255,122);
		A.updateScreen();
		counter++;
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	std::cout << std::endl << 1000.0f*counter/std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-currentimer).count()<< std::endl;
}
