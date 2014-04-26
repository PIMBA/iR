/************************************************************************
*
* Header File Name: IrisViewport
* Class Name : IrisViewport
* Function : This class can limit the showing of all sprites set in which.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once

#include "d3dUtility.h"
#include <list>
#include <vector>

class IrisSprite;
class IrisColor;
class IrisTone;
class IrisRect;

using std::list;

class IrisViewport
{
private:
	list<IrisSprite*> sprites;
	IDirect3DDevice9* Device;
	IDirect3DVertexBuffer9 *renderVertex;
	IDirect3DTexture9 *renderTexture;
	IDirect3DSurface9 *renderSurface;

	void MakeBuffer();
	void MakeTexture();

public:

	list<IrisSprite*> GetSprites();

	void RenderSurface();
	void RenderToBackbuffer();

	IrisRect *rect;
	bool visible;
	int z;
	float ox, oy;
	IrisTone *tone;
	IrisColor *color;

	IrisViewport(float x, float y, float width, float height, IDirect3DDevice9* Device);
	IrisViewport(IrisRect *rect, IDirect3DDevice9* Device);

	static bool compareSpriteWithZ(IrisSprite *v1, IrisSprite *v2);
	void sortViewports();
	void AutoDispose();

	void Dispose();
	bool Disposed();

	void addSprite(IrisSprite* sprite);
	void deleteSprite(IrisSprite* sprite);

	void Flash(IrisColor *color, int duration);
	void Update();

	float GetX();
	float GetY();
	UINT GetWidth();
	UINT GetHeight();

	void Draw();

	~IrisViewport(void);

};

