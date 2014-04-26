/************************************************************************
*
* Header File Name: IrisFont
* Class Name : IrisFont
* Function : This class can symbol font to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once

#include "d3dUtility.h"
class IrisColor;

class IrisFont
{
public:
	IrisFont(LPWSTR name);
	IrisFont(LPWSTR name, int size);

	static bool Existed(LPWSTR name);

	static LPWSTR defaultName;
	static int defaultSize;
	static bool defaultBold;
	static bool defaultItalic;
	static bool defaultShadow;
	static IrisColor *defaultColor;

	LPWSTR name;
	int size;
	bool bold;
	bool italic;
	bool shadow;
	IrisColor *color;

	~IrisFont(void);
};

