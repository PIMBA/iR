/************************************************************************
*
* Header File Name: IrisSprite
* Class Name : IrisSprite
* Function : This class can show graphics onto the screen in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/


#pragma once
#include "d3dUtility.h"

class IrisViewport;
class IrisTone;
class IrisBitmap;

class IrisSprite
{
private:
	IDirect3DDevice9 *Device;
	IrisViewport *viewport;
	int opacity;

public:

	IrisBitmap *bitmap;

	RECT srcRect;

	bool visible;

	float x, y;

	float z;

	float ox, oy;

	float zoomX, zoomY; 

	float angle;

	float waveAmp;
	float waveLength;
	float waveSpeed;
	float wavePhase;
	
	bool mirror;

	int bushDepth;
	int bushOpacity;

	int blendType;

	IrisColor *color;

	IrisTone *tone;

	IrisSprite(IDirect3DDevice9 *Device);
	IrisSprite(IDirect3DDevice9 *Device, IrisViewport *viewport);

	void SetViewport(IrisViewport *tviewport);
	IrisViewport* GetViewport(IrisViewport *tviewport);

    void Dispose();
	bool Disposed();

	void AutoDispose();
	void AutoSetViewport(IrisViewport *tviewport);

	void Flash(IrisColor *color, int duration);

	void Update();

	void Draw();

	int GetWidth();
	int GetHeight();

	void SetOpacity(int tOpacity);
	int GetOpacity();

	~IrisSprite(void);
};

