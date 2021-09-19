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
	/* constructor / destructor */
	MCCScreen();
	MCCScreen(const MCCScreen&) = delete;

	/* useful functions*/
	uint8_t init(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice = "/dev/fb0", const std::string& TTYToClaim = "/dev/tty1");
	void putPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
	void putPixel(uint16_t x, uint16_t y, uint32_t colour);
	void updateScreen();
	void eraseScreen();

	/* destructor */
	~ MCCScreen();

	private:
	struct fb_fix_screeninfo MCCfInfo;
	struct fb_var_screeninfo MCCvInfo;
	int MCCfb, MCCtty;
	uint8_t MCCscreenSelection, MCCScreenError;
	uint64_t MCCscreenSize;
	std::string MCCclaimedTTY;
	char *MCCfbp; //framebuffer pointer
};
#endif

