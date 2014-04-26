#include "IrisSprite.h"
#include <cmath>
using namespace Gdiplus;

const double PI = 3.14159;

IrisSprite::IrisSprite(IDirect3DDevice9 *Device){
	this->Device = Device;
	this->angle = 0.0f;
	this->blendType = 0;
	this->bushDepth = 0;
	this->bushOpacity = 255;
	this->mirror = false;
	this->opacity = 255;
	this->ox = 0.0f;
	this->oy = 0.0f;
	this->visible = true;
	this->waveAmp = 0.0f;
	this->waveLength = 0.0f;
	this->wavePhase = 0.0f;
	this->waveSpeed = 0.0f;
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->zoomX = 1.0f;
	this->zoomY = 1.0f;
	this->viewport = ModuleIrisGraphics::getGViewport(); // 將不受viewport的控制，需要手工釋放.
	this->viewport->addSprite(this);
	this->viewport->sortViewports();
}

IrisSprite::IrisSprite(IDirect3DDevice9 *device, IrisViewport *viewport){
	this->Device = device;

	this->angle = 0.0f;
	this->blendType = 0;
	this->bushDepth = 0;
	this->bushOpacity = 255;
	this->mirror = false;
	this->opacity = 255;
	this->ox = 0.0f;
	this->oy = 0.0f;
	this->visible = true;
	this->waveAmp = 0.0f;
	this->waveLength = 0.0f;
	this->wavePhase = 0.0f;
	this->waveSpeed = 0.0f;
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->zoomX = 1.0f;
	this->zoomY = 1.0f;

	this->viewport = viewport;
	this->viewport->addSprite(this);
	this->viewport->sortViewports();
}

void IrisSprite::Dispose(){
	this->bitmap->Dispose();
	delete this->bitmap;
	this->bitmap = NULL;
	this->viewport->deleteSprite(this);
	this->viewport = NULL;
	this->Device = NULL;
}

bool IrisSprite::Disposed(){
	if(this->Device == NULL)
		return true;
	return false;
}

void IrisSprite::AutoDispose(){
	this->bitmap->Dispose();
	delete this->bitmap;
	this->bitmap = NULL;
	this->viewport = NULL;
	this->Device = NULL;
}

void IrisSprite::Flash(IrisColor *color, int duration){
}

void IrisSprite::Update(){
	if(this->visible)
		return;
		//this->Draw();
}

void IrisSprite::Draw(){
	if(this->bitmap == NULL)
		return;

	if(this->visible == false)
		return;

	this->bitmap->Draw(255, this->viewport, this->x, this->y);
	/*
	D3DXMATRIX proj;
	Device->SetViewport(&(this->viewport->GetD3DViewport()));
	D3DXMatrixOrthoOffCenterLH(&proj, this->viewport->GetX(), this->viewport->GetWidth(), this->viewport->GetHeight(), this->viewport->GetY(), 0.0f, 9999.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	*/
	//-------------Material && Texture Setting-------------
	//this->bitmap->SetOpacity(opacity / 255.0);
	//this->Device->SetMaterial(&(this->bitmap->mtrl));
	Device->SetTexture(0, this->bitmap->GetTexture());

	//-------------Matrix Math-------------

	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	Device->SetTransform(D3DTS_WORLD, &W);
	Device->SetFVF(Iris2DVertex::FVF);

	D3DXMATRIX translationMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX scalingMatrix;
	D3DXMATRIX resultMatrix;

	D3DXMatrixIdentity(&translationMatrix);
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixIdentity(&scalingMatrix);

	if(this->angle != 0.0f)
		D3DXMatrixRotationZ(&rotationMatrix, this->angle);

	D3DXMatrixTranslation(&translationMatrix, this->x, this->y, 0.0f);

	if(this->zoomX != 1.0f || this->zoomY != 1.0f)
		D3DXMatrixScaling(&scalingMatrix, this->zoomX, this->zoomY, 1.0f);
	
	resultMatrix = rotationMatrix * translationMatrix * scalingMatrix;
	Device->SetTransform(D3DTS_WORLD, &resultMatrix);

	Device->SetStreamSource(0, this->bitmap->GetVb(), 0, sizeof(Iris2DVertex));
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

}

int IrisSprite::GetWidth(){
	if(this->bitmap == NULL)
		return 0;
	else
		return this->bitmap->width;
}

int IrisSprite::GetHeight(){
	if(this->bitmap == NULL)
		return 0;
	else
		return this->bitmap->height;
}

void IrisSprite::SetViewport(IrisViewport *tviewport){
	this->viewport->deleteSprite(this);
	this->viewport = tviewport;
	this->viewport->addSprite(this);
}

void IrisSprite::AutoSetViewport(IrisViewport *tviewport){
	this->viewport = tviewport;
	this->viewport->addSprite(this);
}

IrisViewport* IrisSprite::GetViewport(IrisViewport *tviewport){
	return this->viewport;
}

void IrisSprite::SetOpacity(int tOpacity){
	this->opacity = tOpacity;
	this->bitmap->SetOpacity(this->opacity);
}

int IrisSprite::GetOpacity(){
	return this->opacity;
}


IrisSprite::~IrisSprite(void)
{
	if(!this->Disposed())
		this->Dispose();
	if(!this->bitmap->Disposed())
		this->bitmap->Dispose();
}