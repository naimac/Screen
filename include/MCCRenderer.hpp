#include<vector>

union MCCVect
{
	struct
	{
		uint64_t x, y, z;
	};
	uint64_t c[3];
};

struct MCCTriangle
{
	MCCVect p[3];
};

struct MCCMesh
{
	std::vector<MCCTriangle> m;
};

class MMCCamera
{
	public:

	MCCCamera() = delete;
	MCCCamera(MCCVect pos, MCCVect la, MCCVect top);
	const MCCVect& MCCGetPos();
	const MCCVect& MCCGetLookAt();
	const MCCVect& MCCGetTop();
	void MCCSetPos(const MCCVect& newPos);
	void MCCSetLookAt(const MCCVect& newLA);
	void MCCSetTop(const MCCVect& newTop);

	private:

	MCCVect MCCpos;
	MCCVect MCClookAt;
	MCCVect MCCtop;
};

class MCCRenderer
{
	public:

	MCCRenderer() = delete;
	MCCRenderer(MCCCamera &);
	void MCCAddMesh(const MCCMesh& meshToAdd);
	void MCCRender();
	void MCCWipeOut();

	private:

	std::vector<MCCMesh> MCCscenery;
	MCCCamera &MCCcamera;
};
