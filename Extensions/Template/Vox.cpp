#include "common.h"
#include "CfcFile.h"
#include <Windows.h>
#include <mathfu\matrix.h>
#include <mathfu\vector.h>
#include <mathfu\glsl_mappings.h>
#include <math.h>
#include <map>
#include <math.h>

//VoxModel* Vox::GetModel(stdtstring filename) {
//	return nullptr;
//	//std::map<stdtstring, std::vector<VoxModel*>> models = Edif::Runtime::ReadGlobal()
//}

float Vox::LerpAngle(float angle1, float angle2, float alpha) {
	angle1 = Deg2Rad(angle1);
	angle2 = Deg2Rad(angle2);
	auto angleDiff = angle2 - angle1;
	angleDiff = fmod(angleDiff, 2 * PI);
	angleDiff = fmod(angleDiff + 3 * PI, 2 * PI) - PI;
	return Rad2Deg(angle1 + alpha*angleDiff);
}

int Vox::Project(float objx, float objy, float objz, mathfu::mat4 projMatrix, mathfu::mat4 modelMatrix, float winWidth, float winHeight, int* winX, int* winY, float* winZ, int* winX2, int* winY2, bool clamp)
{
	mathfu::vec4 in = projMatrix * modelMatrix * mathfu::vec4(objx, objy, objz, 1.0f);
	in.x = (winWidth * 0.5f) + ((1.0f + in.x) * winWidth / 2.0f);
	in.y = (winHeight * 0.5f) - ((1.0f + in.y) * winHeight / 2.0f);
	in.z = in.z + 1.0f / 2.0f;

	if (clamp) {
		*winX = mathfu::Clamp<int>(RoundUp(in.x), 0, winWidth - 1);
		*winY = mathfu::Clamp<int>(RoundUp(in.y), 0, winHeight - 1);
	}
	else {
		*winX = RoundUp(in.x);
		*winY = RoundUp(in.y);
	}
	*winZ = Clamp01(in.z);

	float remainingX = fmodf(in.x, 1.0f);
	float remainingY = fmodf(in.y, 1.0f);
	if (remainingX != 0.0f || remainingY != 0.0f) {
		if (clamp) {
			*winX2 = mathfu::Clamp<int>(RoundDown(in.x), 0, winWidth - 1);
			*winY2 = mathfu::Clamp<int>(RoundDown(in.y), 0, winHeight - 1);
		}
		else {
			*winX2 = RoundDown(in.x);
			*winY2 = RoundDown(in.y);
		}
		return 2;
	}
	return 1;
}

void Vox::SetVoxPixel(cSurface* source, LPBYTE buffer, LPBYTE alpha, int bufferPitch, int bufferDepth, int alphaPitch, int x, int y, float z, COLORREF color) {
	source->SetPixelFast(x, y, color);
	alpha[(alphaPitch * y) + x] = depthAsAlpha ? unsigned char(z * 255) : 255;
}

void Vox::Draw(int x, int y, int width, int height) {
	if (voxAnimation == nullptr || !voxAnimation->loaded || surface == nullptr) {
		return;
	}
	unsigned int modelIndex = RepeatInt(voxAnimation->numModels, voxAnimation->currentFrame);
	VoxModel* voxModel = &voxAnimation->voxModels[modelIndex];
	unsigned int bufferSize = width * height;
	float* zBuffer = new float[bufferSize];
	for (unsigned int i = 0; i < bufferSize; i++) {
		zBuffer[i] = 0.0f;
	}
	int bufferPitch = surface->GetPitch();
	int bufferDepth = surface->GetDepth();
	int alphaPitch = surface->GetAlphaPitch();
	LPBYTE lockedBuffer = surface->LockBuffer();
	LPBYTE lockedAlpha = surface->LockAlpha();
	unsigned char zero = 0;
	for (int ay = 0; ay < height; ay++) {
		for (int ax = 0; ax < width; ax++) {
			lockedAlpha[(alphaPitch * ay) + ax] = zero;
		}
	}
	mathfu::mat4 projMatrix = GetProjMatrix(voxAnimation->depthSize, width, height);
	mathfu::mat4 modelMatrix = GetModelMatrix(voxAnimation->depthSize);
	for (auto it = voxModel->voxVoxels.begin(); it != voxModel->voxVoxels.end(); it++) {
		VoxModel::VoxVoxel* voxVoxel = it->second;
		if (voxVoxel->invisible) {
			continue;
		}
		COLORREF color = voxAnimation->palette[voxVoxel->i];
		int x1;
		int y1;
		float z;
		int x2;
		int y2;
		int projectedPixels = Project(voxVoxel->x, voxVoxel->y, voxVoxel->z, projMatrix, modelMatrix, width, height, &x1, &y1, &z, &x2, &y2);
		int byteIndex1 = GetIndex(x1, y1, width);
		if (zBuffer[byteIndex1] <= z) {
			zBuffer[byteIndex1] = z;
			SetVoxPixel(surface, lockedBuffer, lockedAlpha, bufferPitch, bufferDepth, alphaPitch, x1, y1, z, color);
		}
		if (projectedPixels > 1) {
			int byteIndex2 = GetIndex(x2, y2, width);
			if (zBuffer[byteIndex2] <= z) {
				zBuffer[byteIndex2] = z;
				SetVoxPixel(surface, lockedBuffer, lockedAlpha, bufferPitch, bufferDepth, alphaPitch, x2, y2, z, color);
			}
		}
	}
	surface->UnlockAlpha();
	surface->UnlockBuffer(lockedBuffer);
	delete[] zBuffer;
}

void Vox::DrawToSurface(cSurface* destination, int x, int y, int width, int height, unsigned char bAntiA, BlitMode bm, BlitOp bo, int effectParam, bool forceSize) {
	Draw(x, y, width, height);
	if (forceSize || localScale != 1.0f) {
		int destX = forceSize ? 0 : -((width * localScale) - width) * 0.5f;
		int destY = forceSize ? 0 : -((height * localScale) - height) * 0.5f;
		int destWidth = forceSize ? width : width * localScale;
		int destHeight = forceSize ? height : height * localScale;
		cSurface scaled;
		scaled.Create(destWidth, destHeight, surface);
		surface->Stretch(scaled, destX, destY, destWidth, destHeight, BMODE_OPAQUE, BOP_COPY, 0, STRF_COPYALPHA);
		scaled.Blit(*destination, x, y, 0, 0, width, height, bm, bo, effectParam);
	} else {
		surface->Blit(*destination, x, y, 0, 0, width, height, bm, bo, effectParam);
	}
}

void Vox::ProjectPosition(float x, float y, float z) {
	if (voxAnimation == nullptr || !voxAnimation->loaded || surface == nullptr) {
		return;
	}
	int x1;
	int y1;
	float z1;
	int x2;
	int y2;
	mathfu::mat4 projMatrix = GetProjMatrix(voxAnimation->depthSize, projectionWidth, projectionHeight);
	mathfu::mat4 modelMatrix = GetModelMatrix(voxAnimation->depthSize);
	int projectedPixels = Project(x, y, z, projMatrix, modelMatrix, projectionWidth, projectionHeight, &x1, &y1, &z1, &x2, &y2, false);
	if (projectedPixels == 1) {
		projectedX = x1;
		projectedY = y1;
	}
	else {
		projectedX = (x2 + x1) / 2;
		projectedY = (y2 + y1) / 2;
	}
	projectedZ = z1;
}

void Vox::SetAnimation(unsigned int animation) {
	if (mV == nullptr || animation < 0 || animation > MaxAnimations) {
		return;
	}
	if (voxAnimations != nullptr) {
		if (!voxAnimations[animation].loaded) {
			voxAnimations[animation].LoadFromFile(mV);
		}
		if (voxAnimations[animation].loaded) {
			if (voxAnimation != nullptr) {
				voxAnimation->currentFrame = 0;
			}
			voxAnimation = &voxAnimations[animation];
		}
	}
}

void Vox::Destroy() {
	if (voxAnimations != nullptr) {
		//for (int i = 0; i < MaxAnimations; i++) {
		//	delete &voxAnimations[i];
		//}
		delete[] voxAnimations;
		voxAnimations = nullptr;
	}
	if (voxAnimation != nullptr) {
		//delete voxAnimation;
		voxAnimation = nullptr;
	}
	mV = nullptr;
}

void Vox::Reset() {
	Destroy();
	voxAnimations = new VoxAnimation[MaxAnimations];
	localAngleX = 0.0f;
	localAngleY = 0.0f;
	localAngleZ = 0.0f;
	localScale = 1.0f;
	hotSpotX = 0;
	hotSpotY = 0;
	hotSpotZ = 0;
	depthAsAlpha = false;
	projectedX = 0;
	projectedY = 0;
	projectedZ = 0.0f;
	projectionWidth = 0;
	projectionHeight = 0;
	animateMovements = false;
	angleOffsetY = 0.0f;
	width = 128;
	height = 128;
}

Vox::Vox()
{
	Reset();
}

Vox::~Vox()
{
	Destroy();
}
