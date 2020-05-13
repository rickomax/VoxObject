/* Expressions.cpp
 * This is where you should define the
 * behavior of your expressions. Make sure
 * the parameters and return types match those
 * in the JSON exactly! Double check Extension.h
 * as well.
 */

#include "Common.h"
float Extension::GetLocalAngleX() {
	return vox->localAngleX;
}

float Extension::GetLocalAngleY() {
	return vox->localAngleY;
}

float Extension::GetLocalAngleZ() {
	return vox->localAngleZ;
}

int Extension::GetCurrentFrame() {
	return vox->voxAnimation == nullptr ? 0 : vox->voxAnimation->currentFrame;
}

int Extension::GetDepthSize() {
	return vox->voxAnimation == nullptr ? 0 : vox->voxAnimation->depthSize;
}

int Extension::GetProjectedX() {
	return vox->projectedX;
}

int Extension::GetProjectedY() {
	return vox->projectedY;
}