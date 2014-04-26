#include "ModuleIrisGraphics.h"

int ModuleIrisGraphics::width;
int ModuleIrisGraphics::height;
IDirect3DDevice9 *ModuleIrisGraphics::Device;

float ModuleIrisGraphics::frameRate;
unsigned long ModuleIrisGraphics::frameCount;
int ModuleIrisGraphics::brightness;

list<IrisViewport*> ModuleIrisGraphics::viewports;
IrisViewport* ModuleIrisGraphics::gViewport;

int ModuleIrisGraphics::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure
	GetImageEncoders(num, size, pImageCodecInfo);
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}   
	}
	free(pImageCodecInfo);
	return -1;  // Failure
}


bool ModuleIrisGraphics::compareViewprotWithZ(IrisViewport *v1, IrisViewport *v2){
	if(v1->z < v2->z)
        return true;
    return false;
}

void ModuleIrisGraphics::sortViewports(){
	ModuleIrisGraphics::viewports.sort(ModuleIrisGraphics::compareViewprotWithZ);
}

ModuleIrisGraphics::ModuleIrisGraphics(void)
{
}

void ModuleIrisGraphics::Init(){
	gViewport = new IrisViewport(0, 0, (float)ModuleIrisGraphics::width, (float)ModuleIrisGraphics::height, Device);
}

void ModuleIrisGraphics::addViewport(IrisViewport *viewport){
	viewports.push_back(viewport);
}

void ModuleIrisGraphics::deleteViewport(IrisViewport *viewport){
	viewports.remove(viewport);
}

IrisViewport* ModuleIrisGraphics::getGViewport(){
	return gViewport;
}

void ModuleIrisGraphics::setDevice(IDirect3DDevice9 *tDevice){
	Device = tDevice;
}

void ModuleIrisGraphics::Update(){
	list<IrisViewport*>::iterator it;

	for(it = viewports.begin(); it != viewports.end(); it++){
		(*it)->RenderSurface();
	}

	D3DXMATRIX proj;
	D3DXMatrixOrthoOffCenterLH(&proj, 0.0f, (float)ModuleIrisGraphics::getWidth(), (float)ModuleIrisGraphics::getHeight(), 0.0f, 0, 9999.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	Device->BeginScene();
	for(it = viewports.begin(); it != viewports.end(); it++){
		(*it)->RenderToBackbuffer();
	}
	Device->EndScene();
}

void ModuleIrisGraphics::disposeAllGraphicsSource(){
	ModuleIrisGraphics::deleteViewport(ModuleIrisGraphics::gViewport);

	list<IrisViewport*>::iterator it;
	for(it = viewports.begin(); it != viewports.end(); it++){
		(*it)->AutoDispose();
	}
	
	list<IrisSprite*> sprites = ModuleIrisGraphics::gViewport->GetSprites();
	list<IrisSprite*>::iterator its = sprites.begin();
	for(its = sprites.begin(); its != sprites.end(); its++){
		(*its)->AutoDispose();
	}
}

void ModuleIrisGraphics::Wait(int duration){
}

void ModuleIrisGraphics::fadeOut(int duration){
}

void ModuleIrisGraphics::fadeIn(int duration){
}

void ModuleIrisGraphics::transition(int duration, LPSTR filename, int vague){
}

void ModuleIrisGraphics::snap2Bitmap(IrisBitmap *bitmap){
}

void ModuleIrisGraphics::frameReset(){
}

void ModuleIrisGraphics::resizeScreen(int width, int height){
}

int ModuleIrisGraphics::getWidth(){
	return width;
}

int ModuleIrisGraphics::getHeight(){
	return height;
}

ModuleIrisGraphics::~ModuleIrisGraphics(void)
{
}
