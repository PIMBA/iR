/************************************************************************
*
* Header File Name: IrisRect
* Class Name : IrisRect
* Function : This class can symbol rect to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once
#include "d3dUtility.h"

using namespace Gdiplus;

class IrisRect
{
public:
	float x, y, width, height;

	IrisRect(float x, float y, float width, float height);

	void Set(float x, float y, float width, float height);

	 Rect* Conver2GDIRect();

	~IrisRect(void);
};

