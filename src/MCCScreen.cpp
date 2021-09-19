#include "MCCScreen.hpp"
#include "MCCErrorCodes.hpp"

/* constructors */
MCCScreen::MCCScreen()
{
}

uint8_t MCCScreen::init(uint16_t height, uint16_t width, uint8_t bpp, const std::string& fbDevice, const std::string& TTYToClaim)
{
	//try to open framebuffer device
	MCCfb = open(fbDevice.c_str(), O_RDWR);
	//get its variable parameters
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
	{
		MCCScreenError = MCC_COULD_NOT_GET_VPARAM;
		return MCCScreenError;
	}
	//set user's parameters
	MCCvInfo.bits_per_pixel = bpp;
	MCCvInfo.xres = width;
	MCCvInfo.yres = height;
	MCCvInfo.xres_virtual = width;
	//enable double buffering
	MCCvInfo.yres_virtual = 2*height;
	MCCscreenSelection = 0;
	MCCvInfo.yoffset = MCCvInfo.yres;
	//update variable parameters, if unsuccessful, return MCC_COULD_NOT_SET_VPARAM
	if(ioctl(MCCfb, FBIOPUT_VSCREENINFO, &MCCvInfo) < 0)
	{
		MCCScreenError = MCC_COULD_NOT_SET_VPARAM;
		return MCCScreenError;
	}
/*	//get new variable parameters
	if(ioctl(MCCfb, FBIOGET_VSCREENINFO, &MCCvInfo) < 0)
	{
		MCCScreenError = MCC_COULD_NOT_GET_NEW_VPARAM;
		return MCCScreenError;
	}
*/	//get fixed parameters
	if(ioctl(MCCfb, FBIOGET_FSCREENINFO, &MCCfInfo) < 0)
	{
		MCCScreenError = MCC_COULD_NOT_GET_FPARAM;
		return MCCScreenError;
	}
	MCCscreenSize = MCCvInfo.yres * MCCfInfo.line_length;
	MCCfbp = (char *)mmap(0, MCCvInfo.yres_virtual*MCCfInfo.line_length, PROT_READ | PROT_WRITE, MAP_SHARED, MCCfb, 0);
	if(MCCfbp==MAP_FAILED)
	{
	 	MCCScreenError = MCC_COULD_NOT_MAP_TO_SCREEN;
		return MCCScreenError;
	}
	//open and claim TTY
	MCCtty = open(TTYToClaim.c_str(), O_RDWR);
	if(ioctl(MCCtty, KDSETMODE, KD_GRAPHICS) < 0)
	{
		MCCScreenError = MCC_COULD_NOT_CLAIM_TTY;
		return MCCScreenError;
	}
	MCCScreenError = MCC_SCREEN_SET_SUCCESS;
	return MCCScreenError;
}

/* destructor */
MCCScreen::~MCCScreen()
{
	switch(MCCScreenError){
		case MCC_SCREEN_SET_SUCCESS:
			ioctl(MCCtty, KDSETMODE, KD_TEXT);
			close(MCCtty);
		case MCC_COULD_NOT_CLAIM_TTY:
			munmap(MCCfbp, 2 * MCCscreenSize);
		case MCC_COULD_NOT_MAP_TO_SCREEN:
		case MCC_COULD_NOT_GET_FPARAM:
		case MCC_COULD_NOT_GET_NEW_VPARAM:
			MCCvInfo.bits_per_pixel = 32;
			MCCvInfo.xres = 1824;
			MCCvInfo.yres = 984;
			MCCvInfo.xres_virtual = 1824;
			MCCvInfo.yres_virtual = 984;
			ioctl(MCCfb, FBIOPUT_VSCREENINFO, &MCCvInfo);
		default:
			close(MCCfb);
			break;
	}
}

/* useful functions*/

void MCCScreen::putPixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	*((uint32_t*)(MCCfbp + (x+MCCvInfo.xoffset) * (MCCvInfo.bits_per_pixel / 8) + (y + MCCvInfo.yres * MCCscreenSelection) * MCCfInfo.line_length)) = (uint32_t)((red << MCCvInfo.red.offset) | (green << MCCvInfo.green.offset) | (blue << MCCvInfo.blue.offset));
}

void MCCScreen::putPixel(uint16_t x, uint16_t y, uint32_t colour)
{
	putPixel(x,y,(colour<<(32-MCCvInfo.red.offset))>>24,(colour<<(32-MCCvInfo.green.offset))>>24,(colour<<(32-MCCvInfo.blue.offset))>>24,(colour<<(32-MCCvInfo.transp.offset))>>24);
}

void MCCScreen::updateScreen()
{
	//swap back buffer and front buffer
	MCCvInfo.yoffset = MCCvInfo.yres * MCCscreenSelection;
	ioctl(MCCfb, FBIO_WAITFORVSYNC, nullptr);
	ioctl(MCCfb, FBIOPAN_DISPLAY, &MCCvInfo);
	MCCscreenSelection++;
	MCCscreenSelection%=2;
}

void MCCScreen::eraseScreen()
{
	memset(MCCfbp + MCCvInfo.xoffset * MCCvInfo.bits_per_pixel / 8 + MCCvInfo.yres * MCCscreenSelection * MCCfInfo.line_length, 0, MCCvInfo.yres * MCCfInfo.line_length);
}
