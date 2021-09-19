#include "MCCScreen.hpp"

/* constructors */
MCCScreen::MCCScreen(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice)
{
	MCCfb = open(fbDevice.c_str(), O_RDWR);
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
	{
		std::cout << "Could not get variable info" << std::endl;
		exit(4);
	}
	MCCvInfo.bits_per_pixel = bpp;
	MCCvInfo.xres = width;
	MCCvInfo.yres = height;
	MCCvInfo.xres_virtual = width;
	MCCvInfo.yres_virtual = 2*height;
	MCCscreenSelection = 0;
	MCCvInfo.yoffset = MCCvInfo.yres;
	if(ioctl(MCCfb, FBIOPUT_VSCREENINFO, &MCCvInfo) < 0)
	{
		std::cout << "Could not set new variable info" << std::endl;
		exit(4);
	}
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
	{
		std::cout << "Could not get new variable info" << std::endl;
		exit(4);
	}
	if(ioctl(MCCfb, FBIOGET_FSCREENINFO, &MCCfInfo) < 0)
	{
		std::cout << "Could not get fixed info" << std::endl;
		exit(4);
	}
	MCCscreenSize = MCCvInfo.yres * MCCfInfo.line_length;
	MCCfbp = (char *)mmap(0, MCCvInfo.yres_virtual*MCCfInfo.line_length, PROT_READ | PROT_WRITE, MAP_SHARED, MCCfb, 0);
	if(MCCfbp==MAP_FAILED)
	{
	 	std::cout << "Could not get map to screen" << std::endl;
		exit(4);
	}
	MCChasTTYBeenClaimed = false;
}

MCCScreen::MCCScreen(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice, const std::string& TTYToClaim)
{
	MCCfb = open(fbDevice.c_str(), O_RDWR);
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
	{
		std::cout << "Could not get variable info" << std::endl;
		exit(4);
	}
	MCCvInfo.bits_per_pixel = bpp;
	MCCvInfo.xres = width;
	MCCvInfo.yres = height;
	MCCvInfo.xres_virtual = width;
	MCCvInfo.yres_virtual = 2*height;
	MCCscreenSelection = 0;
	MCCvInfo.yoffset = MCCvInfo.yres;
	if(ioctl(MCCfb, FBIOPUT_VSCREENINFO, &MCCvInfo) < 0)
	{
		std::cout << "Could not set new variable info" << std::endl;
		exit(4);
	}
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
	{
		std::cout << "Could not get new variable info" << std::endl;
		exit(4);
	}
	if(ioctl(MCCfb, FBIOGET_FSCREENINFO, &MCCfInfo) < 0)
	{
		std::cout << "Could not get fixed info" << std::endl;
		exit(4);
	}
	MCCscreenSize = MCCvInfo.yres * MCCfInfo.line_length;
	MCCfbp = (char *)mmap(0, MCCvInfo.yres_virtual*MCCfInfo.line_length, PROT_READ | PROT_WRITE, MAP_SHARED, MCCfb, 0);
	if(MCCfbp==MAP_FAILED)
	{
	 	std::cout << "Could not get map to screen" << std::endl;
		exit(4);
	}
	MCChasTTYBeenClaimed = true;
	MCCtty = open(TTYToClaim.c_str(), O_RDWR);
	if(ioctl(MCCtty, KDSETMODE, KD_GRAPHICS) < 0)
	{
		std::cout << "Could not claim TTY" << std::endl;
		exit(4);
	}

}

/* destructor */
MCCScreen::~MCCScreen()
{
	if(MCChasTTYBeenClaimed)
	{
		ioctl(MCCtty, KDSETMODE, KD_TEXT);
		close(MCCtty);
	}
	munmap(MCCfbp, 2 * MCCscreenSize);
	MCCvInfo.bits_per_pixel = 32;
	MCCvInfo.xres = 1824;
	MCCvInfo.yres = 984;
	MCCvInfo.xres_virtual = 1824;
	MCCvInfo.yres_virtual = 984;
	ioctl(MCCfb, FBIOPUT_VSCREENINFO, &MCCvInfo);
	close(MCCfb);
}

/* useful functions*/
void MCCScreen::MCCSetScreen(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice, const bool& claimTTY, const std::string& TTYToClaim)
{
	close(MCCfb);
	if(MCChasTTYBeenClaimed)
	{
		ioctl(MCCtty, KDSETMODE, KD_TEXT);
		close(MCCtty);
	}
	munmap(MCCfbp, 2 * MCCscreenSize);
	MCCfb = open(fbDevice.c_str(), O_RDWR);
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
		std::cout << "Could not get variable info" << std::endl;
	if(ioctl(MCCfb, FBIOGET_FSCREENINFO, &MCCfInfo) < 0)
		std::cout << "Could not get fixed info" << std::endl;
	MCCvInfo.bits_per_pixel = bpp;
	MCCvInfo.xres = width;
	MCCvInfo.yres = height;
	MCCvInfo.xres_virtual = width;
	MCCvInfo.yres_virtual = height;
	if(ioctl(MCCfb, FBIOPUT_VSCREENINFO, &MCCvInfo) < 0)
		std::cout << "Could not set new variable info" << std::endl;
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
		std::cout << "Could not get new variable info" << std::endl;
	MCCscreenSize = MCCvInfo.yres_virtual * MCCfInfo.line_length;
	MCCfbp = (char *)mmap(0, MCCscreenSize, PROT_READ | PROT_WRITE, MAP_SHARED, MCCfb, 0);
	if(MCCfbp==MAP_FAILED)
		std::cout << "Could not get map to screen" << std::endl;
	MCChasTTYBeenClaimed = claimTTY;
	if(MCChasTTYBeenClaimed)
	{
		MCCtty = open(TTYToClaim.c_str(), O_RDWR);
		if(ioctl(MCCtty, KDSETMODE, KD_GRAPHICS) < 0)
		{
			std::cout << "Could not claim TTY" << std::endl;
			exit(4);
		}
	}
}

void MCCScreen::putPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	*((uint32_t*)(MCCfbp + (x+MCCvInfo.xoffset) * (MCCvInfo.bits_per_pixel / 8) + (y + MCCvInfo.yres * MCCscreenSelection) * MCCfInfo.line_length)) = (uint32_t)((red << MCCvInfo.red.offset) | (green << MCCvInfo.green.offset) | (blue << MCCvInfo.blue.offset));
}

void MCCScreen::putPixel(uint16_t x, uint16_t y, uint32_t colour)
{
	putPixel(x,y,(colour<<(32-MCCvInfo.red.offset))>>24,(colour<<(32-MCCvInfo.green.offset))>>24,(colour<<(32-MCCvInfo.blue.offset))>>24,(colour<<(32-MCCvInfo.transp.offset))>>24);
}

void MCCScreen::MCCUpdateScreen()
{
	MCCvInfo.yoffset = MCCvInfo.yres * MCCscreenSelection;
	ioctl(MCCfb, FBIOPAN_DISPLAY, &MCCvInfo);
	MCCscreenSelection++;
	MCCscreenSelection%=2;
}

void MCCScreen::MCCEraseScreen()
{
	memset(MCCfbp + MCCvInfo.xoffset * MCCvInfo.bits_per_pixel / 8 + MCCvInfo.yres * MCCscreenSelection * MCCfInfo.line_length, 0, MCCvInfo.yres * MCCfInfo.line_length);
}
