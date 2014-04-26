/************************************************************************
*
* Header File Name: IrisTone
* Class Name : IrisTone
* Function : This class can symbol tone to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once
class IrisTone
{
public:
	float red, green, blue, alpha;

	IrisTone(float red, float green, float blue, float alpha);

	void set(float red, float green, float blue);
	void set(float red, float green, float blue, float alpha);

	~IrisTone(void);
};