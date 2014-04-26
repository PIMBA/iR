/************************************************************************
*
* Header File Name: IrisBitmap
* Class Name : IrisBitmap
* Function : This class can oparate bitmap data to use in this engine.
* Complite Tick: Unkown
* Compliter : Hui
* Dialogure Log :
*    2014.01.02 : Add this introduction.
*
*************************************************************************/

#pragma once
#include "d3dUtility.h"
using namespace std;
using namespace Gdiplus;

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
typedef struct
{
    BYTE  red;              // [0,255]
    BYTE  green;            // [0,255]
    BYTE  blue;             // [0,255]

}COLOR_RGB;

typedef struct
{
    float hue;              // [0,360]
    float saturation;       // [0,100]
    float luminance;        // [0,100]
}COLOR_HSL;

using namespace Gdiplus;

class IrisFont;
class IrisColor;
class IrisRect;
class IrisViewport;

class IrisBitmap
{
private:
	IDirect3DVertexBuffer9 *vb;
	IDirect3DIndexBuffer9 *ib;
	IDirect3DDevice9 *Device;
	IDirect3DTexture9 *texture;
	//D3DMATERIAL9      mtrl;

	Graphics* graphics;

	bool IrisBitmapBitBlt(int x, int y, Bitmap *destBitmap, Bitmap* srcBitmap, Rect *srcRect, int opacity = 255);
	bool IrisBitmapStretchBlt(Rect *destRect, Bitmap* destBitmap, Rect *srcRect, Bitmap* srcBitmap, int opacity = 255);
	Bitmap* ScaleBitmap(Bitmap* srcBitmap, int width, int height);

	void MakeBuffer(int width, int height);
	void MakeTexture(float spriteX, float spriteY, int opacity = 255);

	void SetOpacity(int opacity);

	IrisViewport *inhoodViewport;

	bool needRemakeTexture;

	static void RGBtoHSL(const COLOR_RGB *rgb, COLOR_HSL *hsl);
	static void HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB *rgb);

public:

	friend class IrisSprite;

	Bitmap *bitmap;

	int width, height;
	IrisFont *font;

	IrisBitmap(IDirect3DDevice9 *Device, LPWSTR fileName);
	IrisBitmap(IDirect3DDevice9 *Device, int width, int height);

	void Dispose();
	bool Disposed();

	bool Blt(int x, int y, IrisBitmap *srcIrisBitmap, IrisRect *rect, int opactiy);
	bool StretchBlt(IrisRect *destIrisRect, IrisBitmap *srcIrisBitmap, IrisRect *srcIrisRect, int opacity);

	void FillRect(int x, int y, int width, int height, IrisColor *color);
	void FillRect(IrisRect *rect, IrisColor *color);

	void GradientFillRect(int x, int y, int width, int height, IrisColor *color1, IrisColor *color2, bool vertical = false);
	void GradientFillRect(IrisRect *rect, IrisColor *color1, IrisColor *color2, bool vertical = false);

	void Clear();

	void ClearRect(int x, int y, int width, int height);
	void ClearRect(IrisRect* rect);

	IrisColor* GetPixel(int x, int y) ;

	void SetPixel(int x, int y, IrisColor *color);

	void hueChange(int hue);

	void blur();

	void RadialBlur(float angle, int division);

	int TextSize(IrisFont* fontIris, LPWSTR str);

	void IrisDrawText(IrisFont* fontIris, int x, int y, int width, int height, wstring str, int align);
	void IrisDrawText(IrisFont* fontIris, IrisRect *rect, wstring str, int align);

	IDirect3DVertexBuffer9* GetVb();
	IDirect3DTexture9* GetTexture();

	void Draw(int opacity, IrisViewport *viewport, float spritX, float spriteY);

	IDirect3DDevice9* GetDevice();

	~IrisBitmap(void);
};

