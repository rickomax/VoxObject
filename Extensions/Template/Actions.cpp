/* Actions.cpp
 * This is where you should define the
 * behavior of your actions. Make sure
 * the parameters match those in the JSON
 * exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"
#include "EffectEx.h"

void Extension::SetLocalAngleX(float value) {
	vox->localAngleX = value;
}

void Extension::SetLocalAngleY(float value) {
	vox->localAngleY = value;
}

void Extension::SetLocalAngleZ(float value) {
	vox->localAngleZ = value;
}

void Extension::SetLocalScale(float value) {
	vox->localScale = value;
}

void Extension::SetCurrentFrame(float value) {
	//todo
}

void Extension::ProjectPosition(float x, float y, float z) {
	vox->ProjectPosition(x, y, z);
}

void Extension::LoadLight(unsigned char currentLight, int x, int y, int z, int intensity, unsigned char r, unsigned char g, unsigned char b, unsigned int attenuation) {
	VoxLight::LoadLight(currentLight, x, y, z, intensity, r, g, b, attenuation);
	LPRH rhPtr = rd->rHo.hoAdRunHeader;
	LPOBL oblPtr = (LPOBL)rhPtr->rhObjectList;
	for (int i = 0; i < rhPtr->rhNObjects; oblPtr++, i++)
	{
		LPRO roPtr = (LPRO)(oblPtr->oblOffset);
		if (roPtr == NULL) continue;
		DWORD effectParam = roPtr->ros.rsEffectParam;
		if (roPtr->ros.rsEffect & BOP_EFFECTEX)
		{
			CEffectEx* pEffectEx = (CEffectEx*)effectParam;
			if (pEffectEx != nullptr) {
				int lightSurfaceParamIndex = pEffectEx->GetParamIndex("lightSurface");
				if (lightSurfaceParamIndex >= 0) pEffectEx->SetParamSurfaceValue(lightSurfaceParamIndex, GetSurfaceImplementation(*VoxLight::hwaSurface));
			}
		}
	}
}