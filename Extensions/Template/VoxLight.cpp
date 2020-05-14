#include "VoxLight.h"
#include <math.h>
#include "mathfu\glsl_mappings.h"
cSurface* VoxLight::surface;
cSurface* VoxLight::hwaSurface;

COLORREF VoxLight::EncodeFloatRGB(double v) {
	const double factor = (exp2(24.0) - 1.0) / exp2(24.0);
	mathfu::Vector<double, 3> enc = mathfu::Vector<double, 3>(1.0, 255.0, 255.0 * 255.0) * factor * v;
	enc.x = Frac(enc.x);
	enc.y = Frac(enc.y);
	enc.z = Frac(enc.z);
	enc -= mathfu::Vector<double, 3>(enc.y, enc.z, enc.z) * mathfu::Vector<double, 3>(1.0 / 255.0, 1.0 / 255.0, 0.0);
	return RGB(enc.x * 255, enc.y * 255, enc.z * 255);
}

void VoxLight::LoadLight(unsigned char currentLight, int x, int y, int z, int intensity, unsigned char r, unsigned char g, unsigned char b, unsigned int attenuation)
{
	COLORREF xC = EncodeFloatRGB(x / WorldWidth);
	COLORREF yC = EncodeFloatRGB(y / WorldHeight);
	COLORREF zC = EncodeFloatRGB(z / WorldDepth);
	COLORREF intensityC = EncodeFloatRGB(intensity / MaxIntensity);
	COLORREF rgbC = RGB(r, g, b);
	COLORREF attenuationC = EncodeFloatRGB(attenuation / MaxAttenuation);
	LPBYTE buffer = surface->LockBuffer();
	surface->SetPixelFast(0, currentLight, xC);
	surface->SetPixelFast(1, currentLight, yC);
	surface->SetPixelFast(2, currentLight, zC);
	surface->SetPixelFast(3, currentLight, intensityC);
	surface->SetPixelFast(4, currentLight, rgbC);
	surface->SetPixelFast(5, currentLight, attenuationC);
	surface->UnlockBuffer(buffer);
	surface->Blit(*hwaSurface);
	if (++currentLight >= MaxLights) {
		currentLight = 0;
	}
}

VoxLight::VoxLight()
{

}


VoxLight::~VoxLight()
{
}
