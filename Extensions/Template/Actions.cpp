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