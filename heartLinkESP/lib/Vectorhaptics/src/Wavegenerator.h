#pragma once

#include <startendparam.h>

/** Set to 1 to use stepped (quantized) sweep; 0 for smooth continuous sweep. */
#define STEPPED_SWEEP 0

class BoardProfile;
class VHCoreMultiChnl;
class Wavegenerator
{
private:
	BoardProfile *mBoardPtr;
	VHCoreMultiChnl *mVHCoreMultiChnlPtr;

public:
	Wavegenerator(BoardProfile *ptr, VHCoreMultiChnl *pMulti);
	~Wavegenerator();
	unsigned char vibrate(float frequency, unsigned char intensity, unsigned long duration, float sharpness, unsigned long startTime);
	unsigned char sweep(unsigned long duration, StartEndParam &intensity, StartEndParam &frequency, StartEndParam &sharpness, unsigned long startTime, double *pAccumulatedPhase = nullptr, unsigned long *pLastUpdateTime = nullptr);
	unsigned char pulse(unsigned char intensity, unsigned long duration, unsigned long startTime, float sharpness);
	unsigned char tick(unsigned char intensity, unsigned long duration, unsigned long startTime, float sharpness);
	unsigned char pause(unsigned long duration, unsigned long startTime);
	unsigned char implementation1(unsigned char intensity, unsigned long duration, float sharpness, unsigned long startTime);
	unsigned char implementation2(unsigned char intensity, unsigned long duration, float sharpness, unsigned long startTime);
};
