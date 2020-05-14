#pragma once

#include "Edif.h"
#include <math.h>

#define MaxLights 8
#define WorldWidth 800.0
#define WorldHeight 800.0
#define WorldDepth 800.0
#define MaxIntensity 100.0
#define MaxAttenuation 100.0

#define Frac(a) a - floor(a)

class VoxLight
{
public:
	static cSurface* surface;
	static cSurface* hwaSurface;

	static COLORREF EncodeFloatRGB(double v);
	static void LoadLight(unsigned char currentLight, int x, int y, int z, int intensity, unsigned char r, unsigned char g, unsigned char b, unsigned int attenuation);
	VoxLight();
	~VoxLight();
private:
	static VoxLight voxLights[255];
};

