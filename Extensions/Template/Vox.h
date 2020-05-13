#pragma once
#include "VoxModel.h"
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <mathfu\matrix.h>
#include <mathfu\vector.h>
#include <mathfu\glsl_mappings.h>
#include <math.h>
#include <map>

typedef std::basic_string<TCHAR> stdtstring;

#define MaxAnimations 16
#define MaxFilenameLength 1024
#define RoundUp(a) int(ceil(a))
#define RoundDown(a) int(floor(a))
#define Lerp(amin,amax,value) mathfu::Lerp(amin, amax, value)
#define InverseLerp(amin,amax,value) float((value - amin) / (amax - amin))
#define ClampInt(amin,amax,value) mathfu::Clamp<int>(value,amin,amax)
#define Clamp(amin,amax,value) mathfu::Clamp<float>(value,amin,amax)
#define Clamp01(value) mathfu::Clamp<float>(value, 0.0f, 1.0f)
#define Repeat(amax,value) float(value - floor(value / amax) * amax);
#define RepeatInt(amax,value) unsigned int(value - floor(value / amax) * amax);
#define GetIndex(x,y,width) int((y * width) + x)
#define GetIndexClamped(x,y,width,height) ClampInt(0, width * height, GetIndex(x,y,width))

#define GetProjMatrix(depthSize, width, height) mathfu::mat4::Ortho(0.0f, width, 0.0f, height, 0.1f, depthSize, -1.0f);
#define GetModelMatrix(depthSize) mathfu::mat4::FromTranslationVector(mathfu::vec3(0.0f, 0.0f, depthSize * 0.5f)) * (mathfu::quat::FromAngleAxis(Deg2Rad(localAngleX), mathfu::vec3(1.0f, 0.0f, 0.0f)) * mathfu::quat::FromAngleAxis(Deg2Rad(localAngleY), mathfu::vec3(0.0f, 1.0f, 0.0f)) * mathfu::quat::FromAngleAxis(Deg2Rad(localAngleZ), mathfu::vec3(0.0f, 0.0f, 1.0f))).ToMatrix4() * mathfu::mat4::FromTranslationVector(mathfu::vec3(-hotSpotX, -hotSpotY, -hotSpotZ));

const float OcclusionValue = 1.0f / 12.0f;
const float AlphaMultiplier = 30.0f;

inline float Deg2Rad(float value) {
	return value * (PI / 180.0f);
}

inline float Rad2Deg(float value) {
	return value * (360.0 / (PI * 2.0));
}

class Vox
{
public:
	cSurface* surface = NewSurface();
	LPSMASK* mask = nullptr;
	mv * mV;

	int width;
	int height;
	float localAngleX;
	float localAngleY;
	float localAngleZ;
	float localScale;
	int hotSpotX;
	int hotSpotY;
	int hotSpotZ;
	int projectedX;
	int projectedY;
	float projectedZ;
	int projectionWidth;
	int projectionHeight;
	bool depthAsAlpha;
	stdtstring positionParameterX;
	stdtstring positionParameterY;
	stdtstring depthSizeParameter;
	bool animateMovements;
	float angleOffsetY;
	VoxAnimation* voxAnimations = nullptr;
	VoxAnimation* voxAnimation = nullptr;
	Vox();
	void Destroy();
	void Reset();
	void Draw(int x, int y, int width, int height);
	void DrawToSurface(cSurface* destination, int x, int y, int width, int height, unsigned char bAntiA, BlitMode bm, BlitOp bo, int effectParam, bool forceSize = false);
	void ProjectPosition(float x, float y, float z);
	void SetAnimation(unsigned int animation);
	static float LerpAngle(float from, float to, float time);
	~Vox();
private:
	int Project(float objx, float objy, float objz, mathfu::mat4 projMatrix, mathfu::mat4 modelMatrix, float winWidth, float winHeight, int* winX, int* winY, float* winZ, int* winX2, int* winY2, bool clamp = true);
	void SetVoxPixel(cSurface* source, LPBYTE buffer, LPBYTE alpha, int bufferPitch, int bufferDepth, int alphaPitch, int x, int y, float z, COLORREF color);
};

