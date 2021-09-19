#ifndef __MCC_SCREEN__
#define __MCC_SCREEN__
#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <chrono>
#include <errno.h>
#include <vector>
#include <utility>

class MCCScreen{

	public:
	/* constructors */
	MCCScreen(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice = "/dev/fb0");
	MCCScreen(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice, const std::string& TTYToClaim);
	MCCScreen(const MCCScreen&) = delete;

	/* useful functions*/
	void MCCSetScreen(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice = "/dev/fb0", const bool& claimTTY = false, const std::string& TTYToClaim = "/dev/tty1");
	void putPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	void putPixel(uint16_t x, uint16_t y, uint32_t colour);
	void MCCUpdateScreen();
	void MCCEraseScreen();

	/* destructor */
	~ MCCScreen();

	private:
	struct fb_fix_screeninfo MCCfInfo;
	struct fb_var_screeninfo MCCvInfo;
	int MCCfb, MCCtty;
	uint8_t MCCscreenSelection;
	uint64_t MCCscreenSize;
	bool MCChasTTYBeenClaimed;
	std::string MCCclaimedTTY;
	char *MCCfbp; //framebuffer pointer
};
#endif

