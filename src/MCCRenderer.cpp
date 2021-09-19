MCCCamera::MCCCamera(MCCVect pos, MCCVect la):MCCpos(pos),MCClookAt(la){}

const MCCCamera::MCCVect& MCCGetPos()
{
	return MCCpos;
}

const MCCCamera::MCCVect& MCCGetLookAt();
{
	return MCClookAt;
}

const MCCCamera::MCCVect& MCCGetTop()
{
	return MCCtop;
}

void MCCCamera::MCCSetPos(const MCCVect& newPos);
{
	MCCpos = newPos;
}

void MCCCamera::MCCSetLookAt(const MCCVect& newLA);
{
	MCClookAt = newLA;
}

void MCCCamera::MCCSetTop(const MCCVect& newTop)
{
	MCCtop = newTop;
}

MCCRenderer::MCCRenderer(MCCCamera& cam):MCCcamera(cam){}


