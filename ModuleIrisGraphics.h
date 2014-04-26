/************************************************************************
*
* Header File Name: ModuleIrisAudio
* Class Name :ModuleIrisAudio
* Function : This is a static moudle-like class that will be used globoly which has functions of graphics resource oparating and organizing.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/


#pragma once
#include "d3dUtility.h"
#include <list>

using std::list;

class IrisBitmap;
class IrisViewport;
class IrisSprite;

class ModuleIrisGraphics
{
public:

	static int width;
	static int height;
	static IDirect3DDevice9 *Device;

	static list<IrisViewport*> viewports;
	static IrisViewport* gViewport;

//public:
	ModuleIrisGraphics(void);

	static void Init();
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	static IrisViewport* getGViewport();
	static void setDevice(IDirect3DDevice9 *Device);
	static void Update();
	static void Wait(int duration);
	static void fadeOut(int duration);
	static void fadeIn(int duration);
	static void transition(int duration, LPSTR filename, int vague);
	static void snap2Bitmap(IrisBitmap *bitmap);
	static void frameReset();
	static void resizeScreen(int width, int height);
	static int getWidth();
	static int getHeight();
	static bool compareViewprotWithZ(IrisViewport *v1, IrisViewport *v2);
	static void sortViewports();

	static void disposeAllGraphicsSource();

	static void addViewport(IrisViewport *viewport);
	static void deleteViewport(IrisViewport *viewport);

	static float frameRate;
	static unsigned long frameCount;
	static int brightness;

	virtual ~ModuleIrisGraphics(void) = 0;
};