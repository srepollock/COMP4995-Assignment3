#pragma once
#include "DirectX.h"
#include "basics.h"
#ifndef ENGINE
#define ENGINE
#define DRVERTEX_FLAGS (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
struct MeshStruct {
	ID3DXMesh* mesh;
	std::vector<D3DMATERIAL9> mtrls;
	std::vector<IDirect3DTexture9*> tex;
	D3DXVECTOR3 pos;
	MeshStruct(ID3DXMesh* m, std::vector<D3DMATERIAL9> mtrl, std::vector<IDirect3DTexture9*> t, D3DXVECTOR3 p) {
		mesh = m;
		mtrls = mtrl;
		tex = t;
		pos = p;
	}
};
template<class T> void Release(T t)
{
	if (t)
	{
		t->Release();
		t = 0;
	}
}
template<class T> void Delete(T t)
{
	if (t)
	{
		delete t;
		t = 0;
	}
}
class Engine {
private:
	HWND hWndMain; // handle to the main window
	D3DPRESENT_PARAMETERS parameters; // parameters for the d3d
	LPDIRECT3D9 pD3D = 0; // COM object
	LPDIRECT3DDEVICE9 pDevice = 0; // graphics device
	LPDIRECT3DSURFACE9 pBackBuffer = 0; // back buffer
	PDIRECT3DSURFACE9 BitmapSurface = 0; // Bitmap image
	LPDIRECT3DVERTEXBUFFER9 pVB = 0; // vertext buffers
	int DeviceHeight, DeviceWidth;
	std::vector<MeshStruct> Meshes;
	float radius = 20.0f;
	float angle = (3.0f * D3DX_PI) / 2;
	D3DXMATRIX view;
	D3DMATERIAL9 material;
	D3DLIGHT9 light;
	int selected = -1; // default to camera
public:
	Engine();
	Engine(HWND);
	~Engine();
	int EngineInit();
	LPDIRECT3DDEVICE9& getDevice();
	int EngineRender();
	int EngineLoop();
	int EngineShutdown();
	int InitDirect3DDevice(HWND, int, int, BOOL, D3DFORMAT, LPDIRECT3D9, LPDIRECT3DDEVICE9*);
	int LoadBitmapToSurface(TCHAR*, LPDIRECT3DSURFACE9*, LPDIRECT3DDEVICE9);
	HRESULT ValidateDevice();
	HRESULT RestoreGraphics();
	bool LoadMesh(char * filename, int pos);
	bool RenderMesh(MeshStruct);
	void SetupMatrices();
	void updateCamera(float, float, float);
	void SetLightingDirectional();
	void SetLightingAmbient();
	void SetLightingSpot();
	void SetLightingPoint();
	void setSelected(int);
};
#endif