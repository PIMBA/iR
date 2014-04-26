#include "IrisViewport.h"

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

IrisViewport::IrisViewport(float x, float y, float width, float height, IDirect3DDevice9* tDevice){
	this->rect = new IrisRect(x, y, width, height);
	this->Device = tDevice;
	ModuleIrisGraphics::addViewport(this);
	ModuleIrisGraphics::sortViewports();

	this->visible = true;
	this->z = 0.0f;
	this->ox = this->oy = 0.0f;
	this->tone = NULL;
	this->color = NULL;

	MakeBuffer();
	MakeTexture();
	this->renderSurface = NULL;
}

IrisViewport::IrisViewport(IrisRect *rect, IDirect3DDevice9* tDevice){
	this->rect = rect;
	this->Device = tDevice;

	ModuleIrisGraphics::addViewport(this);
	ModuleIrisGraphics::sortViewports();

	this->visible = true;
	this->z = 0.0f;
	this->ox = this->oy = 0.0f;
	this->tone = NULL;
	this->color = NULL;

	MakeBuffer();
	MakeTexture();
	this->renderSurface = NULL;
}

list<IrisSprite*> IrisViewport::GetSprites(){
	return this->sprites;
}

bool IrisViewport::compareSpriteWithZ(IrisSprite *s1, IrisSprite *s2){
	if(s1->z < s2->z)
        return true;
    return false;
}

void IrisViewport::sortViewports(){
	this->sprites.sort(IrisViewport::compareSpriteWithZ);
}

void IrisViewport::MakeBuffer(){
	HRESULT r = this->Device->CreateVertexBuffer(6 * sizeof(Iris2DVertex), D3DUSAGE_WRITEONLY, Iris2DVertex::FVF, D3DPOOL_MANAGED, &(this->renderVertex), 0);

	if (FAILED(r))
	{
		MessageBox(NULL, L"Create buffer failed!", L"Error", 0);
	}

	Iris2DVertex* v;
	this->renderVertex->Lock(0, 0, (void**)&v, 0);
	v[0] = Iris2DVertex(0.0f,					(float)this->GetHeight(),  0.0f, 0.0f, 1.0f);
	v[1] = Iris2DVertex(0.0f,					  0.0f,					   0.0f, 0.0f, 0.0f);
	v[2] = Iris2DVertex((float)this->GetWidth(),  0.0f,				   	   0.0f, 1.0f, 0.0f);

	v[3] = Iris2DVertex(0.0f,			        (float)this->GetHeight(),  0.0f, 0.0f, 1.0f);
	v[4] = Iris2DVertex((float)this->GetWidth(),  0.0f,					   0.0f, 1.0f, 0.0f);
	v[5] = Iris2DVertex((float)this->GetWidth(),(float)this->GetHeight(),  0.0f, 1.0f, 1.0f);

	this->renderVertex->Unlock();
}

void IrisViewport::MakeTexture(){
	this->Device->CreateTexture(this->GetWidth(), this->GetHeight(), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &(this->renderTexture), NULL) ;

	LPDIRECT3DSURFACE9 pRenderSurface;
	LPDIRECT3DTEXTURE9 text;
	LPDIRECT3DSURFACE9 surf;
	D3DLOCKED_RECT lockbits;

	this->renderTexture->GetSurfaceLevel(0, &pRenderSurface);

	this->Device->CreateTexture(this->GetWidth(),this->GetHeight(), 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &text, NULL);
	text->GetSurfaceLevel(0,&surf);
	this->Device->GetRenderTargetData(pRenderSurface, surf);
	surf->LockRect(&lockbits, NULL, D3DLOCK_READONLY);
	pRenderSurface->UnlockRect();
	PARGBQuad p = (PARGBQuad)(lockbits.pBits);
	PARGBQuad c = 0;
	for(unsigned int x = 0; x < this->GetWidth(); x++){
		for(unsigned int y = 0; y < this->GetHeight(); y++){
			c = p + x + y * lockbits.Pitch / sizeof(ARGBQuad);
			c->Alpha = 0;
			c->Blue = 0;// * opacity / 255 + c2->Blue * (1 - opacity / 255);
			c->Red = 0;// * opacity / 255 + c2->Red * (1 - opacity / 255);
			c->Green = 0; //* opacity / 255 + c2->Green * (1 - opacity / 255);
		}
	}
	text->Release();
	surf->Release();
}

void IrisViewport::Dispose(){
	list<IrisSprite*>::iterator it;
	for(it = this->sprites.begin(); it != this->sprites.end(); it++){
		//(*it)->Dispose();
		(*it)->SetViewport(ModuleIrisGraphics::getGViewport());
	}
	//this->sprites.clear();
	ModuleIrisGraphics::deleteViewport(this);
	d3d::Release<IDirect3DVertexBuffer9*>(this->renderVertex);
	d3d::Release<IDirect3DTexture9*>(this->renderTexture);
}


void IrisViewport::AutoDispose(){
	list<IrisSprite*>::iterator it;
	for(it = this->sprites.begin(); it != this->sprites.end(); it++){
		//(*it)->Dispose();
		(*it)->AutoSetViewport(ModuleIrisGraphics::getGViewport());
	}
	//this->sprites.clear();
	d3d::Release<IDirect3DVertexBuffer9*>(this->renderVertex);
	d3d::Release<IDirect3DTexture9*>(this->renderTexture);
}

bool IrisViewport::Disposed(){
	return this->sprites.empty();
}

void IrisViewport::Flash(IrisColor *color, int duration){
}

void IrisViewport::Update(){ 
	list<IrisSprite*>::iterator it;
	for(it = this->sprites.begin(); it != this->sprites.end(); it++){
		(*it)->Update();
	}
}

void IrisViewport::addSprite(IrisSprite* sprite){
	this->sprites.push_back(sprite);
}

void IrisViewport::deleteSprite(IrisSprite* sprite){
	this->sprites.remove(sprite);
}

void IrisViewport::Draw(){
	if(!this->visible)
		return;

	RenderSurface();
	RenderToBackbuffer();
}

void IrisViewport::RenderSurface(){

	IDirect3DSurface9 *oldSurface;
	//this->Device->SetViewport(&(this->d3dViewport));

	//Make Link
	this->renderTexture->GetSurfaceLevel(0, &(this->renderSurface));

	//Save old surface
	this->Device->GetRenderTarget(0, &oldSurface);

	//Set new render target
	this->Device->SetRenderTarget(0, this->renderSurface);
	D3DXMATRIX proj;
	D3DXMatrixOrthoOffCenterLH(&proj, 0.0f, (float)this->GetWidth(), (float)this->GetHeight(), 0.0f, 0, 9999.0f);

	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

	Device->BeginScene();
	list<IrisSprite*>::iterator it;
	for(it = this->sprites.begin(); it != this->sprites.end(); it++){
		(*it)->Draw();
	}
	Device->EndScene();

	this->Device->SetRenderTarget(0, oldSurface);
}

void IrisViewport::RenderToBackbuffer(){
	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	Device->SetTransform(D3DTS_WORLD, &W);

	D3DXMATRIX translationMatrix;
	D3DXMatrixIdentity(&translationMatrix);
	D3DXMatrixTranslation(&translationMatrix, this->GetX(), this->GetY(), 0.0f);

	Device->SetTransform(D3DTS_WORLD, &translationMatrix);

	Device->SetTexture(0, this->renderTexture);
	Device->SetFVF(Iris2DVertex::FVF);
	Device->SetStreamSource(0, this->renderVertex, 0, sizeof(Iris2DVertex));
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

float IrisViewport::GetX(){
	return this->rect->x;
}

float IrisViewport::GetY(){
	return this->rect->y;
}

UINT IrisViewport::GetWidth(){
	return (UINT)this->rect->width;
}

UINT IrisViewport::GetHeight(){
	return (UINT)this->rect->height;
}

IrisViewport::~IrisViewport(void)
{
	if(!this->Disposed())
		this->Dispose();
}