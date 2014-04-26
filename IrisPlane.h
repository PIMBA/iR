/************************************************************************
*
* Header File Name: IrisPlane
* Class Name : IrisPlane
* Function : This class can symbol plane to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/


#pragma once
#include "d3dUtility.h"
class IrisView;

class IrisPlane
{
public:
	IrisBitmap *bitmap;

	IrisViewport *viewport;

	bool visible;

	float z;

	float ox, oy;

	float zoomX, zoomY; 

	int opacity;

	int blendType;

	IrisColor *color;

	IrisTone *tone;

	IrisPlane(void);
	IrisPlane(IrisViewport viewport);

	bool Disposed();
	void Dispose();

	~IrisPlane(void);
};

