//************************************************
// EffectEx.h
//************************************************

#ifndef _EffectEx_h
#define _EffectEx_h

//**************************
//**************************
// External headers
//**************************
//**************************

#include "SurfaceDefs.h"

//**************************
//**************************
// Forwards
//**************************
//**************************

// external
class FAR cSurface;
class FAR cSurfaceImplementation;

// Types
enum {
	EFFECTPARAM_INT,
	EFFECTPARAM_FLOAT,
	EFFECTPARAM_INTFLOAT4,
	EFFECTPARAM_SURFACE,
};

// Flags
#define EFFECTFLAG_INTERNALMASK		0xF0000000
#define EFFECTFLAG_HASTEXTUREPARAMS	0x10000000
#define EFFECTFLAG_WANTSPIXELSIZE	0x00000001
#define EFFECTFLAG_WANTSMATRIX		0x00000002

typedef struct EFFECTPARAM
{
	int		nValueType;
	union {
		int						nValue;
		float					fValue;
		cSurfaceImplementation*	pTextureSurface;
	};
} EFFECTPARAM;
typedef EFFECTPARAM* LPEFFECTPARAM;

//************************************************
// CEffectEx class

class SURFACES_API CEffectEx
{	
public:
	CEffectEx();
	virtual ~CEffectEx();

	virtual BOOL	Initialize(LPCSTR pFxData, LPARAM lUserParam, DWORD dwFlags, int nParams, LPCSTR pParamNames, LPBYTE pParamTypes);
	virtual BOOL	CreateCompiledEffect(cSurface* psf);
	virtual LPVOID	GetCompiledEffect();
	virtual void	ReleaseCompiledEffect(cSurface* psf);
	virtual BOOL	IsEffectValid(cSurface* psf, LPSTR pErrorTextBuffer, int nBufferSize);

	virtual int		GetParamType(int nParamIdx);
	virtual int		GetParamIndex(LPCSTR pParamName);

	virtual DWORD	GetRGBA();
	virtual DWORD	GetFlags();

	virtual int		GetNParams()	{ return m_nParams; }
	virtual int		GetParamIntValue(int nParamIdx);
	virtual float	GetParamFloatValue(int nParamIdx);
	virtual cSurfaceImplementation*	GetParamSurfaceValue(int nParamIdx);

	virtual void	SetRGBA(DWORD dwRGBA);
	virtual void	SetFlags(DWORD dwFlags);
	virtual void	SetParamIntValue(int nParamIdx, int nParamValue);
	virtual void	SetParamFloatValue(int nParamIdx, float fParamValue);
	virtual void	SetParamSurfaceValue(int nParamIdx, cSurfaceImplementation* pSurfaceValue);

	virtual void	SetBackgroundTexture(int nTexture) { m_nBackgroundTexture = nTexture; }
	virtual int		GetBackgroundTexture() { return m_nBackgroundTexture; }

	virtual LPARAM	GetUserParam();

public:
	DWORD			m_dwRGBA;
	DWORD			m_dwFlags;
	LPCSTR			m_pFxBuf;			// Buffer that contains the ASCII data of the effect file, allocated and freed by caller
	LPVOID			m_pHWAEffect;		// Compiled hardware effect
	short			m_nBackgroundTexture;
	short			m_nParams;
	LPCSTR			m_pParamNames;
	LPEFFECTPARAM	m_pParams;
	cSurfaceImplementation* m_pSf;
	LPARAM			m_lUserParam;
	void*			m_idApp;

//	friend class cD3DScreenSurface;
//	friend class cD3DTextureSurface;
//	friend LPVOID D3DCreateEffect(cD3DScreenSurface* psf, CEffectEx* pEffect);
};

SURFACES_API CEffectEx* WINAPI	NewEffect();
SURFACES_API void WINAPI		DeleteEffect(CEffectEx* pEffect);

/*
Quick documentation about the CEffectEx class
---------------------------------------------

To create an effect :

	CEffectEx* pEffectEx = NewEffect();
	pEffectEx->Initialize(pFxData, lUserParam, dwFlags, nParams, pParamNames, pParamTypes);

	pFxData = pointer to the .fx source code of the shader
	lUserParam = a private value that will be stored in the effect and that you can retrieve with GetUserParam
	dwFlags = flags (I don't remember if there are any, I set it to 0 in MMF apparently)
	nParams = number of parameters
	pParamNames = pointer to a list of parameter names, list of zero-terminated strings
	pParamTypes = pointer to the list of parameter types (EFFECTPARAM_INT, EFFECTPARAM_FLOAT, EFFECTPARAM_INTFLOAT4, EFFECTPARAM_SURFACE)

  Note 1: EFFECTPARAM_INTFLOAT4 is a DWORD converted to float4 (each byte between 0 & 255 is converted to a float value)

  Note 2: pFxData is simply stored in the object, you must not destroy it until the CEffectEx object is destroyed. 
		  I don't remember why I did this like that, it could have been more clever maybe...

  Note 3: effects are not compiled in this function, they are compiled the first time they are used, or after a device has been lost and reset.
		  I think you can use the CreateCompiledEffect function if you want to compile it yourself.
		  If CreateCompiledEffect returns FALSE and you want to see why D3D couldn't compile it, you can use IsEffectValid, 
		  which tries to compile the effect and returns the output text of the D3D function, for example :

					LPSTR pErrorText = (LPSTR)calloc(2000, 1);
					BOOL bValid = pEffectEx->IsEffectValid(m_pPreviewScreenSf, pErrorText, 1999);
					if ( *pErrorText != 0 )
						SetDlgItemText(IDC_EDIT_OUTPUT, pErrorText);

		  (note: IsEffectValid deletes the compiled effect if it was successfully created, it's just a "debug" function)

To delete an effect :

	DeleteEffect(pEffectEx);

	Note: you can also use the new and delete operators to create/delete effects, I'm not sure why I've added the NewEffect/DeleteEffect functions)

To set a parameter value :

	EFFECTPARAM_INT:
	EFFECTPARAM_INTFLOAT4:
		pEffectEx->SetParamIntValue(dw, nValue);

	EFFECTPARAM_FLOAT
		pEffectEx->SetParamFloatValue(dw, fValue);

	EFFECTPARAM_SURFACE
		pEffectEx->SetParamSurfaceValue(dw, pSurfaceImp);

		Note: pSurfaceImp is not a cSurface, it's a cSurfaceImplementation, private class - if you need this, you can retrieve a cSurfaceImplementation from a surface with GetSurfaceImplementation(cSurface& sf).

To tell the shader the texture N is the background texture :

	if ( nBackgroundTexture != 0 )
		pEffectEx->SetBackgroundTexture(nBackgroundTexture);

To set the RGBA coefficient of the shader :

	pEffectEx->SetRGBA(dwRGBA);

To use an effect in a Blit function :

	BlitOp bop = BOP_EFFECTEX;
	LPARAM bopParam = (LPARAM)pEffectEx;


*/

#endif  // _EffectEx_h
