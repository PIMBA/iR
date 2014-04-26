/************************************************************************
*
* Header File Name: D3DUtility
* Function : Define structures and functions to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once

#include <d3dx9.h>
#include <string>
#include <d3d9.h>

#include <windows.h>
#include <GdiPlus.h>
#include <MMSystem.h>

#include "IrisBitmap.h"
#include "IrisViewport.h"
#include "IrisSprite.h"
#include "IrisFont.h"
#include "IrisColor.h"
#include "IrisTone.h"
#include "IrisRect.h"

#include "ModuleIrisAudio.h"
#include "ModuleIrisGraphics.h"
#include "ModuleIrisInput.h"

#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

// Define Max FPS
const float FPSMax = 120.0f;

// Iris2D vertex
struct Iris2DVertex {
	Iris2DVertex() {}
	Iris2DVertex(float x, float y, float z, float u, float v){
		_x = x;
		_y = y;
		_z = z;
		_u = u;
		_v = v;
	}

	float _x, _y, _z;
	float _u, _v;
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
};

namespace d3d{
	bool InitD3D(
		HINSTANCE hInstance,
		int width, int height,
		bool windowed,
		D3DDEVTYPE deviceType,
		IDirect3DDevice9 **device);

	int EnterMsgLoop(
		bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t){
		if (t){
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t){
		if(t){
			delete t;
			t = 0;
		}

	}


	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

	//
	// Lights
	//

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	//
	// Materials
	//

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);
}