#include "IrisBitmap.h"
#include "ModuleIrisGraphics.h"

typedef union
{
    ARGB Color;
    struct
    {
        BYTE Blue;
        BYTE Green;
        BYTE Red;
        BYTE Alpha;
    };
}ARGBQuad, *PARGBQuad;

// Converts RGB to HSL

void IrisBitmap::RGBtoHSL(const COLOR_RGB *rgb, COLOR_HSL *hsl)
{
    float h=0, s=0, l=0;
    // normalizes red-green-blue values
    float r = rgb->red/255.f;
    float g = rgb->green/255.f;
    float b = rgb->blue/255.f;
    float maxVal = max3v(r, g, b);
    float minVal = min3v(r, g, b);

    // hue

    if(maxVal == minVal)
    {
        h = 0; // undefined
    }

    else if(maxVal==r && g>=b)
    {
        h = 60.0f*(g-b)/(maxVal-minVal);
    }

    else if(maxVal==r && g<b)
    {
        h = 60.0f*(g-b)/(maxVal-minVal) + 360.0f;
    }

    else if(maxVal==g)
    {
        h = 60.0f*(b-r)/(maxVal-minVal) + 120.0f;
    }
    else if(maxVal==b)
    {
        h = 60.0f*(r-g)/(maxVal-minVal) + 240.0f;
    }

    // luminance
    l = (maxVal+minVal)/2.0f;
    // saturation

    if(l == 0 || maxVal == minVal)
    {
        s = 0;
    }

    else if(0<l && l<=0.5f)
    {
        s = (maxVal-minVal)/(maxVal+minVal);
    }
    else if(l>0.5f)
    {
        s = (maxVal-minVal)/(2 - (maxVal+minVal)); //(maxVal-minVal > 0)?
    }
    hsl->hue = (h>360)? 360 : ((h<0)?0:h);
    hsl->saturation = ((s>1)? 1 : ((s<0)?0:s))*100;
    hsl->luminance = ((l>1)? 1 : ((l<0)?0:l))*100;
}

// Converts HSL to RGB
void IrisBitmap::HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB *rgb)
{
    float h = hsl->hue;                  // h must be [0, 360]
    float s = hsl->saturation/100.f; // s must be [0, 1]
    float l = hsl->luminance/100.f;      // l must be [0, 1]
    float R, G, B;

    if(hsl->saturation == 0)
    {
        // achromatic color (gray scale)
        R = G = B = l*255.f;
	}
	else
    {
        float q = (l<0.5f)?(l * (1.0f+s)):(l+s - (l*s));
        float p = (2.0f * l) - q;
        float Hk = h/360.0f;
        float T[3];
        T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
        T[1] = Hk;              // Tb
        T[2] = Hk - 0.3333333f; // Tg

        for(int i=0; i<3; i++)
        {
            if(T[i] < 0) T[i] += 1.0f;
            if(T[i] > 1) T[i] -= 1.0f;

            if((T[i]*6) < 1)
            {
                T[i] = p + ((q-p)*6.0f*T[i]);
            }
            else if((T[i]*2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
            {
                T[i] = q;
            }
            else if((T[i]*3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
            {
                T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;
            }
            else T[i] = p;
        }
        R = T[0]*255.0f;
        G = T[1]*255.0f;
        B = T[2]*255.0f;

    }
    rgb->red = (BYTE)((R>255)? 255 : ((R<0)?0 : R));
    rgb->green = (BYTE)((G>255)? 255 : ((G<0)?0 : G));
    rgb->blue = (BYTE)((B>255)? 255 : ((B<0)?0 : B));
}

void IrisBitmap::MakeBuffer(int width, int height){
	HRESULT r = this->Device->CreateVertexBuffer(6 * sizeof(Iris2DVertex), D3DUSAGE_WRITEONLY, Iris2DVertex::FVF, D3DPOOL_MANAGED, &(this->vb), 0);

	Iris2DVertex* v;
	this->vb->Lock(0, 0, (void**)&v, 0);
	v[0] = Iris2DVertex(0.0f,			   (float)height,		   0.0f, 0.0f, 1.0f);
	v[1] = Iris2DVertex(0.0f,			       0.0f,			   0.0f, 0.0f, 0.0f);
	v[2] = Iris2DVertex((float)width,		   0.0f,			   0.0f, 1.0f, 0.0f);

	v[3] = Iris2DVertex(0.0f,		       (float)height,		   0.0f, 0.0f, 1.0f);
	v[4] = Iris2DVertex((float)width,         	   0.0f,		   0.0f, 1.0f, 0.0f);
	v[5] = Iris2DVertex((float)width,      (float)height,		   0.0f, 1.0f, 1.0f);

	this->vb->Unlock();
}


void IrisBitmap::MakeTexture(float spriteX, float spriteY, int opacity){
	//HRESULT r = D3DXCreateTextureFromFile(this->Device, "Leimu.jpg", &(this->texture));
	//return;
	//if(r == D3D_OK)
	//	::MessageBox(0, "1", "1", 0);
	
	
	if(this->texture != 0)
		d3d::Release<IDirect3DTexture9*>(this->texture);

	if(!(this->needRemakeTexture == true) || this->vb == 0){
		if(this->vb != 0)
			d3d::Release<IDirect3DVertexBuffer9*>(this->vb);
		MakeBuffer(this->width, this->height);
	}
	this->needRemakeTexture = false;

	Bitmap *tBitmap = this->bitmap;

	D3DXCreateTexture(this->Device, tBitmap->GetWidth(), tBitmap->GetHeight(),
		1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &(this->texture));
	
	D3DLOCKED_RECT rect;
	this->texture->LockRect(0, &rect, NULL, 0);

	BitmapData bmpdt;
	Rect bpRect(0, 0, tBitmap->GetWidth(), tBitmap->GetHeight());
	this->bitmap->LockBits(&bpRect, ImageLockModeWrite, PixelFormat32bppARGB, &bmpdt);
	
	PARGBQuad p = (PARGBQuad)bmpdt.Scan0;
	PARGBQuad p2 = (PARGBQuad)rect.pBits;

	PARGBQuad c1 = 0;
	PARGBQuad c2 = 0;
	
	for(unsigned int x = 0; x < bmpdt.Width; x++){
		for(unsigned int y = 0; y < bmpdt.Height; y++){
			c1 = p + x + y * bmpdt.Stride / sizeof(ARGBQuad);
			c2 = p2 + x + y * rect.Pitch / sizeof(ARGBQuad);
			c2->Alpha = c1->Alpha;
			c2->Blue = c1->Blue;// * opacity / 255 + c2->Blue * (1 - opacity / 255);
			c2->Red = c1->Red;// * opacity / 255 + c2->Red * (1 - opacity / 255);
			c2->Green = c1->Green; //* opacity / 255 + c2->Green * (1 - opacity / 255);
			//c2->Blue = 0;
			//c2->Red = 0;
			//c2->Green = 0;
		}
	}
	
	//memcpy(rect.pBits, bmpdt.Scan0, bmpdt.Width * bmpdt.Height * 4);

	this->bitmap->UnlockBits(&bmpdt);
	this->texture->UnlockRect(0);

}

IrisBitmap::IrisBitmap(IDirect3DDevice9 *Device, int width, int height){
	this->Device = Device;
	this->font = new IrisFont(IrisFont::defaultName, IrisFont::defaultSize);
	this->width = width;
	this->height = height;
	this->bitmap = new Bitmap(width, height, PixelFormat32bppARGB);
	this->graphics = new Graphics(bitmap);
	this->texture = 0;
	this->vb = 0;
	//this->mtrl = d3d::WHITE_MTRL;
	this->inhoodViewport = 0;
	this->needRemakeTexture = false;
}

IrisBitmap::IrisBitmap(IDirect3DDevice9 *Device, LPWSTR fileName){
	this->Device = Device;
	this->font = new IrisFont(IrisFont::defaultName, IrisFont::defaultSize);
	this->bitmap = new Bitmap(fileName, FALSE);
	this->graphics = new Graphics(this->bitmap);
	this->width = this->bitmap->GetWidth();
	this->height = this->bitmap->GetHeight();
	this->texture = 0;
	this->vb = 0;
	//this->mtrl = d3d::WHITE_MTRL;
	this->inhoodViewport = 0;
	this->needRemakeTexture = false;
}

void IrisBitmap::Dispose(){
	Gdiplus::Color color(0, 0, 0);
	this->graphics->Clear(color);
	delete this->graphics;
	delete this->bitmap;
	d3d::Release<IDirect3DVertexBuffer9*>(this->vb);
	d3d::Release<IDirect3DTexture9*>(this->texture);
	this->texture = 0;
	this->vb = 0;
	this->bitmap = 0;
}

bool IrisBitmap::Disposed(){
	if(this->vb == 0)
		return true;
	return false;
}

Bitmap* IrisBitmap::ScaleBitmap(Bitmap* srcBitmap, int width, int height){
	Bitmap* tpBitmap = new Bitmap(width, height, srcBitmap->GetPixelFormat());
	if(tpBitmap){
		Graphics* g = new Graphics(tpBitmap);
		if(g){
			// use the best interpolation mode
			g->SetInterpolationMode(InterpolationModeHighQualityBicubic);
			g->DrawImage(srcBitmap, 0, 0, width, height);
			delete g;
		}
	}
	return tpBitmap;
}

//start private----
bool IrisBitmap::IrisBitmapBitBlt(int x, int y, Bitmap *destBitmap, Bitmap* srcBitmap, Rect *srcRect, int opacity){
	//fail it if the width or height of src rect is larger than those of dect bimtap.
	if(UINT(srcRect->Width + x) > destBitmap->GetWidth() || UINT(srcRect->Height + y) > destBitmap->GetHeight())
		return false;

	if(opacity > 255)
		opacity = 255;
	if(opacity < 0)
		opacity = 0;

	//----
	Bitmap *bmp1 = srcBitmap;
	BitmapData *bmpdt1 = new BitmapData();
	Rect *rect = srcRect;

	//----
	Bitmap *bmp2 = destBitmap;
	BitmapData* bmpdt2 = new BitmapData();
	Rect *rect2 = new Rect(x, y, srcRect->Width, srcRect->Height);

	//----
	bmp1->LockBits(rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, bmpdt1);
	bmp2->LockBits(rect2, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, bmpdt2);

	PARGBQuad p = (PARGBQuad)bmpdt1->Scan0;
	PARGBQuad p2 = (PARGBQuad)bmpdt2->Scan0;

	// 像素地址 = 基地址 + 单行扫描光栅宽度 * y + 像素长度 * x
	BYTE* tp1 = (BYTE*)p;
	BYTE* tp2 = (BYTE*)p2;

	for (UINT y = 0; y < bmpdt2->Height; y++){
		for (UINT x = 0; x <bmpdt2->Width; x++){
			PARGBQuad c1 = (PARGBQuad)tp1 + x + y * bmpdt1->Stride / sizeof(ARGBQuad);
			PARGBQuad c2 = (PARGBQuad)tp2 + x + y * bmpdt2->Stride / sizeof(ARGBQuad);
			c2->Alpha = c1->Alpha;
			c2->Blue = c1->Blue * opacity / 255 + c2->Blue * (1 - opacity / 255);
			c2->Green = c1->Green * opacity / 255 + c2->Green * (1 - opacity / 255);
			c2->Red = c1->Red * opacity / 255 + c2->Red * (1 - opacity / 255);
		}
	}

	bmp1->UnlockBits(bmpdt1);
	bmp2->UnlockBits(bmpdt2);

	delete bmpdt1;
	delete bmpdt2;
	delete rect2;

	return true;
}

bool IrisBitmap::IrisBitmapStretchBlt(Rect *destRect, Bitmap* destBitmap, Rect *srcRect, Bitmap* srcBitmap, int opacity){
	//safety check
	
	if(srcBitmap->GetWidth() < UINT(srcRect->Width + srcRect->X) || srcBitmap->GetHeight() < UINT(srcRect->Height + srcRect->Y))
		return false;
	if(destBitmap->GetWidth() < UINT(destRect->Width + destRect->X) || destBitmap->GetHeight() < UINT(destRect->Height + destRect->Y))
		return false;
	
	if(opacity > 255)
		opacity = 255;
	if(opacity < 0)
		opacity = 0;

	// 先获取截取位图
	Bitmap *tpBitmap = new Bitmap(srcRect->Width, srcRect->Height, PixelFormat32bppARGB);
	IrisBitmapBitBlt(0, 0, tpBitmap, srcBitmap, srcRect, 255);

	// 再缩放之
	Bitmap *scaleBitmap = ScaleBitmap(tpBitmap, destRect->Width, destRect->Height);

	// 复制过去
	Rect *rect = new Rect(0, 0, scaleBitmap->GetWidth(), scaleBitmap->GetHeight());
	IrisBitmapBitBlt(destRect->X, destRect->Y, destBitmap, scaleBitmap, rect, opacity);

	delete tpBitmap;
	delete rect;
	return true;
}
//----end private

bool IrisBitmap::Blt(int x, int y, IrisBitmap *srcIrisBitmap, IrisRect *srcIrisRect, int opacity = 255){
	return IrisBitmapBitBlt(x, y, this->bitmap, srcIrisBitmap->bitmap, srcIrisRect->Conver2GDIRect(), opacity);
}

bool IrisBitmap::StretchBlt(IrisRect *destIrisRect, IrisBitmap *srcIrisBitmap, IrisRect *srcIrisRect, int opacity){
	return IrisBitmapStretchBlt(destIrisRect->Conver2GDIRect(), this->bitmap, srcIrisRect->Conver2GDIRect(), srcIrisBitmap->bitmap, opacity);
}

void IrisBitmap::FillRect(int x, int y, int width, int height, IrisColor *color){
	SolidBrush *tpBrush = new SolidBrush(Color(color->red, color->green, color->blue));
	graphics->FillRectangle(tpBrush, x, y, width, height);
	delete tpBrush;
}

void IrisBitmap::FillRect(IrisRect *rect, IrisColor *color){
	SolidBrush *tpBrush = new SolidBrush(Color(color->red, color->green, color->blue));
	graphics->FillRectangle(tpBrush, *rect->Conver2GDIRect());
	delete tpBrush;
}

void IrisBitmap::GradientFillRect(int x, int y, int width, int height, IrisColor *color1, IrisColor *color2, bool vertical){
}

void IrisBitmap::GradientFillRect(IrisRect *rect, IrisColor *color1, IrisColor *color2, bool vertical){
}

void IrisBitmap::Clear(){
	graphics->Clear(Color(0, 0, 0));
}

void IrisBitmap::ClearRect(int x, int y, int width, int height){
	SolidBrush *tpBrush = new SolidBrush(Color(0, 0, 0));
	graphics->FillRectangle(tpBrush, x, y, width, height);
	delete tpBrush;
}

void IrisBitmap::ClearRect(IrisRect* rect){
	SolidBrush *tpBrush = new SolidBrush(Color(0, 0, 0));
	graphics->FillRectangle(tpBrush, *rect->Conver2GDIRect());
	delete tpBrush;
}

IrisColor* IrisBitmap::GetPixel(int x, int y){
	Color color;
	this->bitmap->GetPixel(x, y, &color);
	return new IrisColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
}

void IrisBitmap::SetPixel(int x, int y, IrisColor *color){
	Color *gdiColor = new Color(color->red, color->green, color->blue, color->alpha);
	this->bitmap->SetPixel(x, y, *gdiColor);
	delete gdiColor;
}

void IrisBitmap::hueChange(int hue){
	BitmapData bmpdt;
	Rect rect(0, 0, this->bitmap->GetWidth(), this->bitmap->GetHeight());
	this->bitmap->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bmpdt);

	PARGBQuad p = (PARGBQuad)bmpdt.Scan0;
    INT offset = bmpdt.Stride - bmpdt.Width * sizeof(ARGBQuad);

	BYTE* tp = (BYTE*)p;

	for(UINT y = 0; y < bmpdt.Height; y++, tp += offset){

		for(UINT x = 0; x < bmpdt.Width; x++, p++){
			COLOR_RGB colorRGB = {p->Red, p->Green, p->Blue};
			COLOR_HSL colorHSL = {0, 0, 0};

			IrisBitmap::RGBtoHSL(&colorRGB, &colorHSL);
			colorHSL.hue = hue;
			IrisBitmap::HSLtoRGB(&colorHSL, &colorRGB);

			p->Red = colorRGB.red;
			p->Green = colorRGB.green;
			p->Blue = colorRGB.blue;
		}
	}

	this->bitmap->UnlockBits(&bmpdt);
}

void IrisBitmap::blur(){
}

void IrisBitmap::RadialBlur(float angle, int division){
}

int IrisBitmap::TextSize(IrisFont* fontIris, LPWSTR str){
	int size = IrisFont::defaultSize;
	IrisColor *color = IrisFont::defaultColor;
	LPWSTR name;
	bool bold, shadow, italic;

	if(fontIris == NULL){
		name = IrisFont::defaultName;
		bold = IrisFont::defaultBold;
		shadow = IrisFont::defaultShadow;
		italic = IrisFont::defaultItalic;
	}
	else
	{
		name = fontIris->name;
		bold = fontIris->bold;
		shadow = fontIris->shadow;
		italic = fontIris->italic;
	}
	
	wstring text(str);

	FontFamily fontFamily(name);
	int fs = FontStyleRegular;

	if(bold)
		fs |= FontStyleBold;

	if(italic)
		fs |= FontStyleItalic;

	Font font(&fontFamily, size, fs, UnitWorld);

	RectF boundRect;

	graphics->MeasureString(str, text.length(), &font, PointF(0, 0), &boundRect);

	return (int)boundRect.Width;
}


void IrisBitmap::IrisDrawText(IrisFont* fontIris, int x, int y, int width, int height, wstring str, int align){
	
	this->needRemakeTexture = true;

	Gdiplus::Bitmap *tBitmap = this->bitmap->Clone(0, 0, this->bitmap->GetWidth(), this->bitmap->GetHeight(), PixelFormat32bppARGB);
	
	Graphics tg(tBitmap);

	int size = IrisFont::defaultSize;
	IrisColor *color = IrisFont::defaultColor;
	LPWSTR name;
	bool bold, shadow, italic;

	if(fontIris == NULL){
		name = IrisFont::defaultName;
		bold = IrisFont::defaultBold;
		shadow = IrisFont::defaultShadow;
		italic = IrisFont::defaultItalic;
	}
	else
	{
		name = fontIris->name;
		bold = fontIris->bold;
		shadow = fontIris->shadow;
		italic = fontIris->italic;
	}
	
	FontFamily fontFamily(name);
	int fs = FontStyleRegular;

	if(bold)
		fs |= FontStyleBold;

	if(italic)
		fs |= FontStyleItalic;

	Font font(&fontFamily, size, fs, UnitWorld);

	StringFormat strFormat;

	switch(align){
	case 0:
		strFormat.SetAlignment(StringAlignmentNear);
		break;
	case 1:
		strFormat.SetAlignment(StringAlignmentCenter);
		break;
	}
	
	Brush *brush;

	if(shadow)
		brush = new HatchBrush(HatchStyle90Percent, Color(color->red, color->green, color->blue));
	else
		brush = new SolidBrush(Color(color->red, color->green, color->blue));
	
	Color keyColor;
	if(color->red == 0 || color->blue == 0 || color-> green == 0)
		keyColor = Color::MakeARGB(255, 255, 255, 255);
	else
		keyColor = Color::MakeARGB(255, color->red - 1, color->green - 1, color->blue - 1);

	PointF point(x, y);
	strFormat.SetFormatFlags(StringFormatFlagsNoWrap);
	RectF r(x, y, width, height);
	tg.DrawString(str.c_str(), str.length(), &font, r, &strFormat, brush);

	if(this->bitmap != 0){
		delete this->bitmap;
		this->bitmap = 0;
	}

	this->bitmap = tBitmap->Clone(0, 0, tBitmap->GetWidth(), tBitmap->GetHeight(), PixelFormat32bppARGB);
	
	delete brush;
	delete tBitmap;
	
}

void IrisBitmap::IrisDrawText(IrisFont* fontIris, IrisRect *rect, wstring str, int align){
	IrisDrawText(fontIris, rect->x, rect->y, rect->width, rect->height, str, align);
}

void IrisBitmap::Draw(int opacity, IrisViewport *tViewport, float spriteX, float spriteY){
	if(this->inhoodViewport != tViewport || this->needRemakeTexture == true){
		this->inhoodViewport = tViewport;
		this->MakeTexture(spriteX, spriteY, opacity);
	}
}

IDirect3DVertexBuffer9* IrisBitmap::GetVb(){
	return this->vb;
}

IDirect3DTexture9* IrisBitmap::GetTexture(){
	return this->texture;
}

IDirect3DDevice9* IrisBitmap::GetDevice(){
	return this->Device;
}

void IrisBitmap::SetOpacity(int opacity){
	//this->mtrl.Diffuse.a = opacity;
	//----
	Bitmap *bmp1 = this->bitmap;
	BitmapData *bmpdt1 = new BitmapData();
	Rect rect(0, 0, this->width, this->height);

	//----
	bmp1->LockBits(&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, bmpdt1);

	PARGBQuad p = (PARGBQuad)bmpdt1->Scan0;

	// 像素地址 = 基地址 + 单行扫描光栅宽度 * y + 像素长度 * x
	BYTE* tp1 = (BYTE*)p;

	for (UINT y = 0; y < bmpdt1->Height; y++){
		for (UINT x = 0; x <bmpdt1->Width; x++){
			PARGBQuad c1 = (PARGBQuad)tp1 + x + y * bmpdt1->Stride / sizeof(ARGBQuad);
			c1->Alpha = c1->Alpha * opacity / 255;
		}
	}

	bmp1->UnlockBits(bmpdt1);

	delete bmpdt1;
}

IrisBitmap::~IrisBitmap(void)
{
	if(!this->Disposed())
		this->Dispose();
}