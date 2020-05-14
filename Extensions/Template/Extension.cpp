/* Extension.cpp
 * This file contains the definitions for
 * your extension's general runtime functions,
 * such as the constructor and destructor,
 * handling routines, etc.
 * Functions defined here:
 * Extension::Extension <constructor>
 * Extension::~Extension <destructor>
 * Extension::Handle
 * Extension::Display
 * Extension::Pause
 * Extension::Continue
 * Extension::Save
 * Extension::Load
 * Extension::Action		<--|
 * Extension::Condition		<--|- not what you think!
 * Extension::Expression	<--|
 */

#include "Common.h"
#include "EffectEx.h"
#include <comdef.h>  

 /* <constructor>
  * This is your extension's constructor, which
  * is the replacement for the old CreateRunObject
  * function. You don't need to manually call
  * constructors or pointlessly initialize
  * pointers with dynamic memory. Just link
  * your A/C/Es, perform initialization steps, and
  * you're good to go.
  */
Extension::Extension(RD *rd, SerializedED *SED, createObjectInfo *COB)
	: rd(rd)
	, rh(rd->rHo.hoAdRunHeader)
	, Runtime(rd)
{
	//Link all your action/condition/expression functions
	//to their IDs to match the IDs in the JSON here.
	LinkAction(0, SetLocalAngleX);
	LinkAction(1, SetLocalAngleY);
	LinkAction(2, SetLocalAngleZ);
	LinkAction(3, SetLocalScale);
	LinkAction(4, SetCurrentFrame);
	LinkAction(5, ProjectPosition);
	LinkAction(6, LoadLight);
	
	LinkCondition(0, IsProjectedZBehind);
	LinkCondition(1, IsProjectedZInTheFront)

	LinkExpression(0, GetLocalAngleX);
	LinkExpression(1, GetLocalAngleY);
	LinkExpression(2, GetLocalAngleZ);
	LinkExpression(3, GetCurrentFrame);
	LinkExpression(4, GetDepthSize);
	LinkExpression(5, GetProjectedX);
	LinkExpression(6, GetProjectedY);

	LPRH rhPtr = rd->rHo.hoAdRunHeader;
	mv* mV = rhPtr->rh4.rh4Mv;

	vox = new Vox();
	EditData ed(SED, mV, false, vox);

	LPSURFACE windowSurface = WinGetSurface((int)rhPtr->rhIdEditWin);
	LPSURFACE proto = NULL;
	GetSurfacePrototype(&proto, windowSurface->GetDepth(), ST_MEMORYWITHDC, SD_DIB);
	vox->surface = NewSurface();
	vox->surface->Create(vox->width, vox->height, proto);
	vox->surface->CreateAlpha();

	VoxLight::surface = NewSurface();
	VoxLight::surface->Create(8, 8, proto);

	LPSURFACE hwaProto = NULL;
	GetSurfacePrototype(&hwaProto, windowSurface->GetDepth(), ST_HWA_ROMTEXTURE, SD_D3D9);
	VoxLight::hwaSurface = NewSurface();
	VoxLight::hwaSurface->Create(8, 8, hwaProto);
	//VoxLight::surface->CreateAlpha();
	
	rd->rHo.hoImgWidth = vox->width;
	rd->rHo.hoImgHeight = vox->height;
}

/* <destructor>
 * This is your extension's destructor, the
 * replacement for DestroyRunObject. No calling
 * destructors manually or deallocating pointless
 * dynamic memory - in most cases this function
 * won't need any code written.
 */
Extension::~Extension()
{
	//
}

/* Handle
 * Fusion calls this function to let your extension
 * "live" - if you want, you can have Fusion call this
 * every frame. This is where you'd, for instance,
 * simulate physics or move an object. This is
 * the analagous function to the old HandleRunObject.
 */
short Extension::Handle()
{
	if (vox->animateMovements) {
		float desiredAngle = ((rd->rc->rcDir / 32.0f) * 360.0f) - vox->angleOffsetY;
		vox->localAngleY = Vox::LerpAngle(vox->localAngleY, desiredAngle, 0.25f);
		if (vox->voxAnimation != nullptr) {
			if (rd->rc->rcSpeed > 0) {
				vox->SetAnimation(rd->rc->rcAnim);
			}
			else {
				vox->SetAnimation(0);
			}
			float speedFactor = InverseLerp((float)rd->rc->rcMinSpeed, (float)rd->rc->rcMaxSpeed, (float)rd->rc->rcSpeed);
			float frameInterval = (1.0f / mathfu::Lerp<float>(vox->voxAnimation->minSpeed, vox->voxAnimation->maxSpeed, speedFactor)) * 1000.0f;
			float passedFrames = rd->rHo.hoAdRunHeader->rhTimerDelta / frameInterval;
			vox->voxAnimation->currentFrame += passedFrames;
		}
	}
	return REFLAG_DISPLAY;
}

/* Display
 * This is the analagous function to
 * DisplayRunObject. If you return
 * REFLAG_DISPLAY in Handle() this
 * routine will run. If you want Fusion
 * to apply ink effects for you, then
 * implement GetRunObjectSurface in
 * Runtime.cpp instead.
 */
short Extension::Display()
{
	if (vox->voxAnimation == nullptr || !vox->voxAnimation->loaded) {
		return 0;
	}

	LPRH rhPtr = rd->rHo.hoAdRunHeader;
	LPSURFACE windowSurface = WinGetSurface((int)rhPtr->rhIdEditWin);

	RECT* rect = &rd->rHo.hoRect;

	int x = rect->left;
	int y = rect->top;
	int width = rd->rHo.hoImgWidth;
	int height = rd->rHo.hoImgHeight;

	BlitMode bm = (rd->rs->rsEffect & EFFECTFLAG_TRANSPARENT) ? BMODE_TRANSP : BMODE_OPAQUE;
	BOOL bAntiA = (rd->rs->rsEffect & EFFECTFLAG_ANTIALIAS) ? TRUE : FALSE;
	BlitOp bo = (BlitOp)(rd->rs->rsEffect & EFFECT_MASK);
	DWORD effectParam = rd->rs->rsEffectParam;

	if (rd->rs->rsEffect & BOP_EFFECTEX && vox->depthAsAlpha)
	{
		CEffectEx* pEffectEx = (CEffectEx*)effectParam;
		if (pEffectEx != nullptr) {
#ifdef _UNICODE
			_bstr_t a(vox->positionParameterX.c_str());
			_bstr_t b(vox->positionParameterY.c_str());
			_bstr_t c(vox->depthSizeParameter.c_str());
			int positionXParamIndex = pEffectEx->GetParamIndex(a);
			int positionYParamIndex = pEffectEx->GetParamIndex(b);
			int depthSizeParamIndex = pEffectEx->GetParamIndex(c);
#else
			int positionXParamIndex = pEffectEx->GetParamIndex(vox->positionParameterX.c_str());
			int positionYParamIndex = pEffectEx->GetParamIndex(vox->positionParameterY.c_str());
			int depthSizeParamIndex = pEffectEx->GetParamIndex(vox->depthSizeParameter.c_str());
#endif
			if (positionXParamIndex >= 0) pEffectEx->SetParamFloatValue(positionXParamIndex, x);
			if (positionYParamIndex >= 0) pEffectEx->SetParamFloatValue(positionYParamIndex, y);
			if (depthSizeParamIndex >= 0) pEffectEx->SetParamFloatValue(depthSizeParamIndex, vox->voxAnimation->depthSize);
			int lightSurfaceParamIndex = pEffectEx->GetParamIndex("lightSurface");
			if (lightSurfaceParamIndex >= 0) pEffectEx->SetParamSurfaceValue(lightSurfaceParamIndex, GetSurfaceImplementation(*VoxLight::hwaSurface));
		}
	}

	vox->DrawToSurface(windowSurface, x, y, width, height, bAntiA, bm, bo, (int)effectParam, false);
	WinAddZone(rhPtr->rhIdEditWin, rect);

	return 0;
}

/* Pause
 * If your extension plays sound, for
 * example, then Fusion calls this to
 * let you know to pause the music,
 * usually by another extension's request
 * or by the player pausing the applcation.
 */
short Extension::Pause()
{
	return 0;
}

/* Continue
 * Opposite to the above, Fusion lets
 * you know that the silence is over;
 * your extension may live again.
 */
short Extension::Continue()
{
	return 0;
}

/* Save
 * When the user uses the Save
 * Frame Position action, you need
 * so serialize your runtime data to
 * the File given. It is a Windows
 * file handle, but you can use some
 * of Fusion's built-in functions for
 * writing files. Check the MMF2SDK
 * Help file for more information.
 */
bool Extension::Save(HANDLE File)
{
	return true;
}

/* Load
 * As opposed to above, here you need to
 * restore your extension's runtime state
 * from the given file. Only read what you
 * wrote!
 */
bool Extension::Load(HANDLE File)
{
	return true;
}


/* Action, Condition, Expression
 * These are called if there's no function linked
 * to an ID. You may want to put MessageBox calls
 * to let you know that the ID is unlinked, or you
 * may just want to use unlinked A/C/Es as a feature.
 */
void Extension::Action(int ID, RD *rd, long param1, long param2)
{
}

long Extension::Condition(int ID, RD *rd, long param1, long param2)
{
	return false; //hopefully StringComparison (PARAM_CMPSTRING) is not used, or this may crash
}

long Extension::Expression(int ID, RD *rd, long param)
{
	return long(_T("")); //so that unlinked expressions that return strings won't crash
}
