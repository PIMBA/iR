#include "d3dUtility.h"

#include <atlbase.h>
#include <sstream>

IDirect3DDevice9 *Device = 0;

DWORD FramCnt = 0;
float TimeElapsed = 0;
float FPS = 0;

IrisSprite* sprite;
IrisSprite* sprite2;
IrisSprite* sprite3;
IrisSprite* s;
IrisViewport *viewport;

const int Width = 800;
const int Height = 600;

void CalcFPS(float timeDelta){

	if(!(sprite2->bitmap->Disposed())){
		sprite2->bitmap->Dispose();
		sprite2->bitmap = new IrisBitmap(Device, 600, 50);
	}

	FPS = 1000.0f / timeDelta;

	wostringstream ost;
	ost << FPS;
	wstring str(ost.str());
	wstring str2 = L"FPS:";

	sprite2->bitmap->IrisDrawText(NULL, 0, 0, 100, 500, str2 + str, 0);

}

bool Setup(){
	// Initialization function calling
	ModuleIrisGraphics::width = Width;
	ModuleIrisGraphics::height = Height;
	ModuleIrisGraphics::Device = Device;
	ModuleIrisGraphics::Init();
	
	sprite3 = new IrisSprite(Device);
	viewport = new IrisViewport(50.0f, 50.0f, 200, 300, Device);
	sprite3->SetViewport(viewport);
	sprite3->bitmap = new IrisBitmap(Device, L"kurumi.jpg");
	//sprite3->SetOpacity(50.0f);

	sprite2 = new IrisSprite(Device);
	viewport = new IrisViewport(0.0f, 0.0f, 600, 50, Device);
	sprite2->SetViewport(viewport);
	sprite2->bitmap = new IrisBitmap(Device, 600, 50);

	sprite = new IrisSprite(Device);
	sprite->bitmap = new IrisBitmap(Device, L"Leimu.jpg");
	sprite->x = 20;
	sprite->y = 80;
	sprite->zoomX = 0.70f;
	sprite->bitmap->IrisDrawText(NULL, 0, 0, 100, 100, L"Hello!", 0);


	/*for(int i = 0;i <= 300;i++)
	{
		s = new IrisSprite(Device);
		viewport = new IrisViewport(50.0f, 50.0f, 200, 300, Device);
		s->SetViewport(viewport);
		sprite->bitmap = new IrisBitmap(Device, L"kurumi.jpg");
		s->angle += i/100;
	}*/


	//-------------Projection Matrix Setting-------------
	D3DXMATRIX proj;
	//Device->SetViewport(&(this->d3dViewport));
	//D3DXMatrixOrthoOffCenterLH(&proj, this->GetX(), this->GetWidth(), this->GetHeight(), this->GetY(), 0.0f, 9999.0f);
	D3DXMatrixOrthoOffCenterLH(&proj, 0, (float)Width, (float)Height, 0, 0.0f, 9999.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//-------------State Setting-------------	
	Device->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE); //¿¹¾â³Ý
	
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	Device->SetRenderState(D3DRS_LIGHTING, false);

	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	
	// use alpha channel in texture for alpha
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// set blending factors so that alpha component determines transparency
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return true;
}

bool Cleanup(){
	//delete sprite;
	ModuleIrisGraphics::disposeAllGraphicsSource();
	return true;
}

bool Display(float timeDelta){
	// Display the sprites
	if(Device){
		//Alpha enabled
		
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		sprite->angle += 0.01;
		sprite3->angle -= 0.02;
		//sprite->x += 1.0f;
		//sprite3->x -= 1.0f;
		CalcFPS(timeDelta);
		ModuleIrisGraphics::Update();

		//Alpha disabled
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		Device->Present(0, 0, 0, 0);

	} 
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);

}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd){

		GdiplusStartupInput m_gdiplusStartupInput;
		ULONG_PTR m_pGdiToken;
		GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

		if(!d3d::InitD3D(hInstance, Width, Height, true, D3DDEVTYPE_HAL, &Device)){
			::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
			return 0;
		}
		
		if (!Setup()){
			::MessageBox(0, L"Setup() - FAILED", 0, 0);
			return 0;
		}

		d3d::EnterMsgLoop(Display);
		 
		Cleanup();

		Device->Release();

		GdiplusShutdown(m_pGdiToken);
		return 0;
}