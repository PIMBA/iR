/************************************************************************
*
* Header File Name: IrisColor
* Class Name : IrisColor
* Function : This class can symbol color to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once
class IrisColor
{
public:
	int red, green, blue, alpha;

	IrisColor(int red, int green, int blue, int alpha);

	void set(int red, int green, int blue);
	void set(int red, int green, int blue, int alpha);

	~IrisColor(void);
};

