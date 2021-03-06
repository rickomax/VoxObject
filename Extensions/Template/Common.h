/* Common.h
 * This is the common header file included
 * by almost all other *.cpp files in
 * the project. Anything that gets put or
 * included in here will be available to you
 * everywhere else. You shouldn't need to
 * change anything in this file except
 * to include headers that you want and
 * to change the edition of Fusion you want
 * to support.
 */

#pragma once

//Only define one of these at once:
//	#define TGFEXT	//TGF2.0, MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
	#define MMFEXT	//        MMF2.0, CTF2.5, MMF2.0 Dev, CTF2.5 Dev
//	#define PROEXT	//                        MMF2.0 Dev, CTF2.5 Dev

#include "Edif.h"
#include "Resource.h"

#include <cstdint>
#include "VoxModel.h"
#include "VoxLight.h"
#include "VoxAnimation.h"
#include "Vox.h"

/* stdtstring
 * A std::string that knows if you're using
 * unicode or not. (Protip: strings passed
 * to your extension A/C/Es still need to be
 * const TCHAR *, and the same goes for strings
 * returned from expressions and conditions).
 */
typedef std::basic_string<TCHAR> stdtstring;

#include "EditData.hpp"
#include "Extension.hpp"