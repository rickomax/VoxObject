/* Properties.cpp
* In this file you will define the edittime
* properties for your extension. By default
* only a version number is displayed.
* Functions defined here:
* GetProperties
* ReleaseProperties
* GetPropCreateParam
* ReleasePropCreateParam
* GetPropValue
* SetPropValue
* GetPropCheck
* SetPropCheck
* EditProp
* IsPropEnabled
*/

#include "Common.h"
//#include "Vox.h"

#ifndef RUN_ONLY

/* Prop
* This is the namespace the property identifiers
* are stored in. It keeps them out of the global
* namespace and it helps with intellisense.
*/
namespace Prop
{
	/* <enum>
	* The property identitifers.
	*/
	enum
	{
		zNOT_USED = PROPID_EXTITEM_CUSTOM_FIRST,
		Group1,
		Group2,
		Group3,
		Group4,
		Group5,
		Version,
		LocalAngleX,
		LocalAngleY,
		LocalAngleZ,
		LocalScale,
		HotSpotX,
		HotSpotY,
		HotSpotZ,
		DepthAsAlpha,
		PositionVariableX,
		PositionVariableY,
		DepthSizeVariable,
		AnimateMovement,
		AngleOffsetY,
		Animations
	};
}

PropData Properties[] = //See the MMF2SDK help file for information on PropData_ macros.
{
	PropData_EditButton(Prop::Animations, (UINT_PTR)_T("Animations"),	(UINT_PTR)_T("Animations")),
	PropData_End()
};

PropData PosProperties[] = {
	PropData_Group(Prop::Group1, (UINT_PTR)_T("Angles"), (UINT_PTR)_T("Angles")),
	PropData_EditFloat(Prop::LocalAngleX, (UINT_PTR)_T("Local Angle X"), (UINT_PTR)_T("Local Angle X")),
	PropData_EditFloat(Prop::LocalAngleY, (UINT_PTR)_T("Local Angle Y"), (UINT_PTR)_T("Local Angle Y")),
	PropData_EditFloat(Prop::LocalAngleZ, (UINT_PTR)_T("Local Angle Z"), (UINT_PTR)_T("Local Angle Z")),
	PropData_Group(Prop::Group2, (UINT_PTR)_T("Scale"), (UINT_PTR)_T("Scale")),
	PropData_EditFloat(Prop::LocalScale, (UINT_PTR)_T("Local Scale"),(UINT_PTR)_T("Local Scale")),
	PropData_Group(Prop::Group3, (UINT_PTR)_T("Hot Spot"), (UINT_PTR)_T("Hot Spot")),
	PropData_EditNumber(Prop::HotSpotX, (UINT_PTR)_T("Hot Spot X"),	(UINT_PTR)_T("Hot Spot X")),
	PropData_EditNumber(Prop::HotSpotY, (UINT_PTR)_T("Hot Spot Y"),	(UINT_PTR)_T("Hot Spot Y")),
	PropData_EditNumber(Prop::HotSpotZ, (UINT_PTR)_T("Hot Spot Z"),	(UINT_PTR)_T("Hot Spot Z")),
	PropData_End()
};

PropData DisplayProperties[] =
{
	PropData_Group(Prop::Group5, (UINT_PTR)_T("Animations"), (UINT_PTR)_T("Animations")),
	PropData_CheckBox(Prop::AnimateMovement, (UINT_PTR)_T("Animate Movements"), (UINT_PTR)_T("Animate Movements")),
	PropData_EditFloat(Prop::AngleOffsetY, (UINT_PTR)_T("Angle Offset Y"),	(UINT_PTR)_T("Angle Offset Y")),
	PropData_Group(Prop::Group4, (UINT_PTR)_T("Shader Adapter"), (UINT_PTR)_T("Shader Adapter")),
	PropData_CheckBox(Prop::DepthAsAlpha, (UINT_PTR)_T("Output Depth as Alpha"), (UINT_PTR)_T("Output Depth as Alpha")),
	PropData_EditString(Prop::PositionVariableX, (UINT_PTR)_T("PositionVariableX"), (UINT_PTR)_T("PositionVariableX")),
	PropData_EditString(Prop::PositionVariableY, (UINT_PTR)_T("PositionVariableY"), (UINT_PTR)_T("PositionVariableY")),
	PropData_EditString(Prop::DepthSizeVariable, (UINT_PTR)_T("DepthSizeVariable"), (UINT_PTR)_T("DepthSizeVariable")),
	PropData_End()
};

#endif

/* GetProperties
* Here, you choose what proeprties to insert
* into which tabs. Lots of dynamic things can
* be done in here, such as dynamically
* generating the properties based on the
* number of stored values in the editdata.
*/
BOOL MMF2Func GetProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	mvInsertProps(mV, SED, Properties, PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, SED, PosProperties, PROPID_TAB_SIZEPOS, TRUE);
	mvInsertProps(mV, SED, DisplayProperties, PROPID_TAB_DISPLAY, TRUE);
	//if you changed ed:
	//ed.Serialize(mV, SED);
	return TRUE;
#endif
	return FALSE;
}

/* ReleaseProperties
* If you dynamically allocated memory for
* property arrays in GetProperties, release
* that memory in this function.
*/
void MMF2Func ReleaseProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropCreateParam
* A convenience function if you use the
* hard-coded Properties array above. Allows
* you to dynamically create e.g. combo
* lists and other things that have
* additional parameters for their
* properties. However if you dynamically
* allocated everything in GetProperties,
* this function is useless.
*/
LPARAM MMF2Func GetPropCreateParam(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//return (LPARAM)TheParameter;
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return 0;
}

/* ReleasePropCreateParam
* If you found a useful way to use the
* above function and you dynamically
* allocated memory, release it here.
*/
void MMF2Func ReleasePropCreateParam(mv *mV, SerializedED *SED, UINT PropID, LPARAM lParam)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//MyAwesomeMemoryFreeingFunction(lParam);
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropValue
* This is where you return the display
* values for the properties you defined.
* Refer to the MMF2SDK Help file for
* information on what to return for
* each kind of property.
*/
void *MMF2Func GetPropValue(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	EditData ed(SED, mV, true);

	switch (PropID)
	{
	case Prop::LocalAngleX: {
		return new CPropFloatValue(ed.vox->localAngleX);
	}
	case Prop::LocalAngleY: {
		return new CPropFloatValue(ed.vox->localAngleY);
	}
	case Prop::LocalAngleZ: {
		return new CPropFloatValue(ed.vox->localAngleZ);
	}
	case Prop::LocalScale: {
		return new CPropFloatValue(ed.vox->localScale);
	}
	case Prop::HotSpotX: {
		return new CPropIntValue(ed.vox->hotSpotX);
	}
	case Prop::HotSpotY: {
		return new CPropIntValue(ed.vox->hotSpotY);
	}
	case Prop::HotSpotZ: {
		return new CPropIntValue(ed.vox->hotSpotZ);
	}
	case Prop::PositionVariableX: {
		return new CPropStringValue(ed.vox->positionParameterX.c_str());
	}
	case Prop::PositionVariableY: {
		return new CPropStringValue(ed.vox->positionParameterY.c_str());
	}
	case Prop::DepthSizeVariable: {
		return new CPropStringValue(ed.vox->depthSizeParameter.c_str());
	}
	case Prop::AngleOffsetY: {
		return new CPropFloatValue(ed.vox->angleOffsetY);
	}
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return NULL;
}

/* SetPropValue
* The user has just finished entering their
* 500 page essay into your MyString field.
* Fusion isn't so kind as to save that for them,
* so it entrusts it with you. Store it!
*/
void MMF2Func SetPropValue(mv *mV, SerializedED *SED, UINT PropID, CPropValue *PropVal)
{
#ifndef RUN_ONLY
	EditData ed(SED, mV);
	switch (PropID)
	{
	case Prop::LocalAngleX: {
		ed.vox->localAngleX = ((CPropFloatValue*)PropVal)->m_fValue;
		break;
	}
	case Prop::LocalAngleY: {
		ed.vox->localAngleY = ((CPropFloatValue*)PropVal)->m_fValue;
		break;
	}
	case Prop::LocalAngleZ: {
		ed.vox->localAngleZ = ((CPropFloatValue*)PropVal)->m_fValue;
		break;
	}
	case Prop::LocalScale: {
		ed.vox->localScale = ((CPropFloatValue*)PropVal)->m_fValue;
		break;
	}
	case Prop::HotSpotX: {
		ed.vox->hotSpotX = ((CPropIntValue*)PropVal)->m_nValue;
		break;
	}
	case Prop::HotSpotY: {
		ed.vox->hotSpotY = ((CPropIntValue*)PropVal)->m_nValue;
		break;
	}
	case Prop::HotSpotZ: {
		ed.vox->hotSpotZ = ((CPropIntValue*)PropVal)->m_nValue;
		break;
	}
	case Prop::PositionVariableX: {
		ed.vox->positionParameterX = ((CPropStringValue*)PropVal)->GetString();
		break;
	}
	case Prop::PositionVariableY: {
		ed.vox->positionParameterY = ((CPropStringValue*)PropVal)->GetString();
		break;
	}
	case Prop::DepthSizeVariable: {
		ed.vox->depthSizeParameter = ((CPropStringValue*)PropVal)->GetString();
		break;
	}
	case Prop::AngleOffsetY: {
		ed.vox->angleOffsetY = ((CPropFloatValue*)PropVal)->m_fValue;
		break;
	}
	}
	ed.Serialize(mV, SED);
	mvInvalidateObject(mV, SED);
#endif
}

/* GetPropCheck
* There are checkbox properties, and
* there are properties that have their
* own checkbox as an option. This is
* where you tell Fusion whether those
* boxes are ticked or not.
*/
BOOL MMF2Func GetPropCheck(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	EditData ed(SED, mV, true);
	switch (PropID)
	{
	case Prop::DepthAsAlpha:
	{
		return ed.vox->depthAsAlpha ? TRUE : FALSE;
	}
	case Prop::AnimateMovement:
		return ed.vox->animateMovements ? TRUE : FALSE;
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* SetPropCheck
* The user has painstakingly moved the
* mouse cursor over the checkbox and
* expended the immense effort required
* to click the mouse and toggle the
* state of the tickbox. Don't let their
* effort be all for naught!
*/
void MMF2Func SetPropCheck(mv *mV, SerializedED *SED, UINT PropID, BOOL Ticked)
{
#ifndef RUN_ONLY
	EditData ed(SED, mV);
	switch (PropID)
	{
	case Prop::DepthAsAlpha:
	{
		ed.vox->depthAsAlpha = (Ticked != FALSE ? true : false);
		break;
	}
	case Prop::AnimateMovement:
		ed.vox->animateMovements = (Ticked != FALSE ? true : false);
		break;
	}
	ed.Serialize(mV, SED);
	mvInvalidateObject(mV, SED);
#endif
}

const static stdtstring AnimationNames[] = {
	u8"Stop",
	u8"Walk",
	u8"Run",
	u8"Appear",
	u8"Disappear",
	u8"Bounce",
	u8"Shoot",
	u8"Jump",
	u8"Fall",
	u8"Climb",
	u8"Crouch",
	u8"Uncrouch",
	u8"Custom 1",
	u8"Custom 2",
	u8"Custom 3",
	u8"Custom 4"
};

Vox* vox = nullptr;
int selectedIndex = -1;

#ifndef RUN_ONLY
BOOL CALLBACK SetupProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG: {
		HWND hwndOwner;
		RECT rc, rcDlg, rcOwner;
		if ((hwndOwner = GetParent(hwndDlg)) == NULL)
		{
			hwndOwner = GetDesktopWindow();
		}
		GetWindowRect(hwndOwner, &rcOwner);
		GetWindowRect(hwndDlg, &rcDlg);
		CopyRect(&rc, &rcOwner);
		OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
		OffsetRect(&rc, -rc.left, -rc.top);
		OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
		SetWindowPos(hwndDlg,
			HWND_TOP,
			rcOwner.left + (rc.right / 2),
			rcOwner.top + (rc.bottom / 2),
			0, 0,
			SWP_NOSIZE);
		HWND hwndList = GetDlgItem(hwndDlg, IDC_ANIMATIONS);
		for (int i = 0; i < ARRAYSIZE(AnimationNames); i++)
		{
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)AnimationNames[i].c_str());
		}
		return TRUE;
	}
	case WM_COMMAND: {
		switch (wmCommandID)
		{
		case IDC_ANIMATIONS: {
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				if (vox == nullptr) {
					return FALSE;
				}
				HWND hwndList = GetDlgItem(hwndDlg, IDC_ANIMATIONS);
				int index = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
				selectedIndex = index;
				VoxAnimation * selectedAnimation = &vox->voxAnimations[selectedIndex];
				SetDlgItemText(hwndDlg, IDC_FILENAME, selectedAnimation->filename.c_str());
				SetDlgItemInt(hwndDlg, IDC_MINSPEED, selectedAnimation->minSpeed, false);
				SetDlgItemInt(hwndDlg, IDC_MAXSPEED, selectedAnimation->maxSpeed, false);
				return TRUE;
			}
			}
			break;
		}
		case IDC_CLEARFILE: {
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
			{
				if (selectedIndex < 0) {
					return FALSE;
				}
				VoxAnimation * selectedAnimation = &vox->voxAnimations[selectedIndex];
				selectedAnimation->filename = "";
				SetDlgItemText(hwndDlg, IDC_FILENAME, "");
				break;
			}
			}
			break;
		}
		case IDC_SELECTFILE: {
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
			{
				if (selectedIndex < 0) {
					return FALSE;
				}
				OPENFILENAME ofn;
				char szFileName[MAX_PATH] = "";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwndDlg;
				ofn.lpstrFilter = "Vox Files (*.vox)\0*.vox";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;
				ofn.lpstrDefExt = "vox";
				if (GetOpenFileName(&ofn))
				{
					VoxAnimation * selectedAnimation = &vox->voxAnimations[selectedIndex];
					selectedAnimation->filename = stdtstring(ofn.lpstrFile);
					vox->SetAnimation(selectedIndex);
					SetDlgItemText(hwndDlg, IDC_FILENAME, selectedAnimation->filename.c_str());
					return TRUE;
				}
			}
			}
			break;
		}
		case IDC_MINSPEED: {
			switch (HIWORD(wParam)) {
			case EN_CHANGE: {
				if (selectedIndex < 0) {
					return FALSE;
				}
				VoxAnimation * selectedAnimation = &vox->voxAnimations[selectedIndex];
				selectedAnimation->minSpeed = GetDlgItemInt(hwndDlg, IDC_MINSPEED, NULL, false);
				return TRUE;
			}
			}
			break;
		}
		case IDC_MAXSPEED: {
			switch (HIWORD(wParam)) {
			case EN_CHANGE: {
				if (selectedIndex < 0) {
					return FALSE;
				}
				VoxAnimation * selectedAnimation = &vox->voxAnimations[selectedIndex];
				selectedAnimation->maxSpeed = GetDlgItemInt(hwndDlg, IDC_MAXSPEED, NULL, false);
				return TRUE;
			}
			}
			break;
		}
		case IDOK:
		case IDCANCEL: {
			EndDialog(hwndDlg, wmCommandID);
			return TRUE;
		}
		}
		break;
	}
	default: {
		break;
	}
	}
	return FALSE;
}
#endif

/* EditProp
* If you use a button proeprty or a
* property that has a button, then
* you would be impolite to ignore
* the user when they click on that
* button. Here is where you respond
* to that button press e.g. with
* a dialog.
*/
BOOL MMF2Func EditProp(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY

	switch (PropID)
	{
	case Prop::Animations:
	{
		EditData ed(SED, mV);
		vox = ed.vox;
		if (DialogBoxParam(hInstLib, MAKEINTRESOURCE(IDD_EDIT), mV->mvHEditWin, SetupProc, 0) == IDOK) {
			ed.Serialize(mV, SED);
			return TRUE;
		}
		selectedIndex = -1;
		vox = nullptr;
		break;
	}
	}

#endif
	return FALSE;
}

/* IsPropEnabled
* Let's say you're tired of having to deal
* with the user's various 500 page essays,
* tickbox toggles, and button clicks. Here,
* you can simply disable a property by
* returning FALSE for that property.
*/
BOOL MMF2Func IsPropEnabled(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	switch (PropID)
	{
	default: {
		return TRUE;
	}
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}
