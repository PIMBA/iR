#include "IrisTone.h"

IrisTone::IrisTone(float red, float green, float blue, float alpha = 0){
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
}

void IrisTone::set(float red, float green, float blue){
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void IrisTone::set(float red, float green, float blue, float alpha){
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
}

IrisTone::~IrisTone(void)
{
}
