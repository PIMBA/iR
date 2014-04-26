#include "IrisFont.h"

LPWSTR IrisFont::defaultName = L"ËÎÌו";
int IrisFont::defaultSize = 20;
bool IrisFont::defaultBold = false;
bool IrisFont::defaultItalic = false;
bool IrisFont::defaultShadow = false;
IrisColor *IrisFont::defaultColor = new IrisColor(255, 255, 255, 255);

IrisFont::IrisFont(LPWSTR name)
{
	this->italic = IrisFont::defaultItalic;
	this->bold = IrisFont::bold;
	this->color = IrisFont::color;
	this->name = name;
	this->shadow = IrisFont::defaultShadow;
	this->size = IrisFont::defaultSize;
}

IrisFont::IrisFont(LPWSTR name, int size){
	this->italic = IrisFont::defaultItalic;
	this->bold = IrisFont::bold;
	this->color = IrisFont::color;
	this->name = name;
	this->shadow = IrisFont::defaultShadow;
	this->size = size;
}

IrisFont::~IrisFont(void)
{
}
