#include "Engine.h"
Engine::Engine() {
	
}
Engine::Engine(HWND hWnd) {
	this->hWndMain = hWnd;
}
Engine::~Engine(){

}
int Engine::EngineInit() {
	HRESULT r = 0;//return values
	D3DSURFACE_DESC desc;
	LPDIRECT3DSURFACE9 pSurface = 0;

	this->pD3D = Direct3DCreate9(D3D_SDK_VERSION);//COM object
	if (this->pD3D == NULL) {
		SetError(_T("Could not create IDirect3D9 object"));
		return E_FAIL;
	}
	// FALSE = full screen TRUE = windowed
	r = InitDirect3DDevice(this->hWndMain, SCREEN_WIDTH, SCREEN_HEIGHT, TRUE, D3DFMT_X8R8G8B8, this->pD3D, &this->pDevice);
	if (FAILED(r)) {//FAILED is a macro that returns false if return value is a failure - safer than using value itself
		SetError(_T("Initialization of the device failed"));
		return E_FAIL;
	}

	// This is where I setup the surface
	r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	pSurface->GetDesc(&desc);

	r = this->pDevice->CreateOffscreenPlainSurface(desc.Width, desc.Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &this->BitmapSurface, NULL);

	r = LoadBitmapToSurface(_T(BITMAP_PICTURE), &pSurface, this->pDevice);
	if (FAILED(r))
		SetError(_T("Error loading bitmap"));

	r = D3DXLoadSurfaceFromSurface(this->BitmapSurface, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
	if (FAILED(r))
		SetError(_T("did not copy surface"));

	LoadMesh("box.x", 0);
	LoadMesh("harrier.x", 1);
	LoadMesh("harrier.x", 2);

	SetupMatrices();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}
LPDIRECT3DDEVICE9& Engine::getDevice() {
	return pDevice;
}
int Engine::EngineRender() {
	HRESULT r;
	this->pBackBuffer = 0;

	if (!this->pDevice) {
		SetError(_T("Cannot render because there is no device"));
		return E_FAIL;
	}
	this->pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 55), 1.0f, 0);
	r = ValidateDevice();
	if (FAILED(r)) {
		return E_FAIL;
	}
	r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->pBackBuffer);
	if (FAILED(r)) {
		SetError(_T("Couldn't get backbuffer"));
	}
	// NULL for fullscreen
	r = this->pDevice->UpdateSurface(this->BitmapSurface, NULL, this->pBackBuffer, NULL);
	if (FAILED(r)) {
		SetError(_T("Error setting surface to back surface"));
	}

	pDevice->SetFVF(DRVERTEX_FLAGS);

	if (selected != -1) { // Object selecte
		if (::GetAsyncKeyState('A') & 0x8000f)
			Meshes[selected].pos.x -= 0.2f;
		else if (::GetAsyncKeyState('D') & 0x8000f)
			Meshes[selected].pos.x += 0.2f;
		else if (::GetAsyncKeyState('S') & 0x8000f)
			Meshes[selected].pos.y -= 0.2f;
		else if (::GetAsyncKeyState('W') & 0x8000f)
			Meshes[selected].pos.y += 0.2f;
	}
	else { // Camera selected
		if (::GetAsyncKeyState('A') & 0x8000f) // left
			updateCamera(-1, 0, 0);
		else if (::GetAsyncKeyState('D') & 0x8000f) // right
			updateCamera(1, 0, 0);
		else if (::GetAsyncKeyState('S') & 0x8000f) // down
			updateCamera(0, -1, 0);
		else if (::GetAsyncKeyState('W') & 0x8000f) // up
			updateCamera(0, 1, 0);
		else if (::GetAsyncKeyState('Q') & 0x8000f) // rot y ccw // TODO Look in the book
			printf("ok");
		else if (::GetAsyncKeyState('E') & 0x8000f) // rot y cw
			printf("ok");
		else if (::GetAsyncKeyState('F') & 0x8000f) // rot z ccw
			printf("ok");
		else if (::GetAsyncKeyState('G') & 0x8000f) // rot z cw
			printf("ok");
		else if (::GetAsyncKeyState('Z') & 0x8000f) // zoom out
			printf("ok");
		else if (::GetAsyncKeyState('X') & 0x8000f) // zoom in
			printf("ok");
		else if (::GetAsyncKeyState(WM_MOUSEWHEEL) & 0x8000f) { // rot x
			if (WHEEL_DELTA > 0) {
				// rot x in

			}
			else {
				// rot x out

			}
		}
		else if (::GetAsyncKeyState(WM_MOUSEMOVE) && 0x8000f) { // change view
			
		}
	}

	//Start to render in 3D
	this->pDevice->BeginScene();

	for (auto m : Meshes) {
		RenderMesh(m);
	}

	// finish rendering
	this->pDevice->EndScene();
												 //console work
	this->pDevice->Present(NULL, NULL, NULL, NULL);//swap over buffer to primary surface
	return S_OK;
}
int Engine::EngineLoop() {
	this->EngineRender();

	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage(0);

	return S_OK;
}
int Engine::EngineShutdown() {
	if (this->pVB)
		this->pVB->Release();
	for (auto m : Meshes) { // release all the meshes
		Release<ID3DXMesh*>(m.mesh);
	}
	if (pBackBuffer) {
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	if (pDevice) {
		pDevice->Release();
		pDevice = NULL;
	}
	return S_OK;
}
int Engine::InitDirect3DDevice(HWND hWndTarget, int Width, int Height, BOOL bWindowed, D3DFORMAT FullScreenFormat, LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9* ppDevice) {
	D3DPRESENT_PARAMETERS d3dpp;//rendering info
	D3DDISPLAYMODE d3ddm;//current display mode info
	HRESULT r = 0;

	if (*ppDevice)
		(*ppDevice)->Release();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	r = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(r)) {
		SetError("Could not get display adapter information");
		return E_FAIL;
	}

	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = bWindowed ? d3ddm.Format : FullScreenFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//D3DSWAPEFFECT_COPY;
	d3dpp.hDeviceWindow = hWndTarget;
	d3dpp.Windowed = bWindowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = 0;//default refresh rate
	d3dpp.PresentationInterval = bWindowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	r = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWndTarget, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, ppDevice);
	if (FAILED(r)) {
		SetError("Could not create the render device");
		return E_FAIL;
	}
	return S_OK;
}
int Engine::LoadBitmapToSurface(TCHAR* PathName, LPDIRECT3DSURFACE9* ppSurface, LPDIRECT3DDEVICE9 pDevice) {
	HRESULT r;
	HBITMAP hBitmap;
	BITMAP Bitmap;

	hBitmap = (HBITMAP)LoadImage(NULL, PathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap == NULL) {
		SetError(_T("Unable to load bitmap"));
		return E_FAIL;
	}

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);
	DeleteObject(hBitmap);//we only needed it for the header info to create a D3D surface

						  //create surface for bitmap
	r = pDevice->CreateOffscreenPlainSurface(Bitmap.bmWidth, Bitmap.bmHeight, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, ppSurface, NULL);

	if (FAILED(r)) {
		SetError(_T("Unable to create surface for bitmap load"));
		return E_FAIL;
	}
	//load bitmap onto surface
	r = D3DXLoadSurfaceFromFile(*ppSurface, NULL, NULL, PathName, NULL, D3DX_DEFAULT, 0, NULL);
	if (FAILED(r)) {
		SetError(_T("Unable to laod file to surface"));
		return E_FAIL;
	}

	return S_OK;
}
HRESULT Engine::ValidateDevice() {
	HRESULT r = 0;
	//Test current state of device
	r = this->pDevice->TestCooperativeLevel();
	if (FAILED(r)) {
		//if device is lost then return failure
		if (r == D3DERR_DEVICELOST)
			return E_FAIL;
		//if device is ready to be reset then try
		if (r == D3DERR_DEVICENOTRESET) {
			//release back surface
			this->pBackBuffer->Release();
			//reset device
			r = this->pDevice->Reset(&this->parameters);
			if (FAILED(r)) {
				//device was not ready to be reset
				SetError(_T("Could not reset device"));
				PostQuitMessage(E_FAIL);
				return E_FAIL;
			}
			//reacquire a pointer to new back buffer
			r = this->pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &this->pBackBuffer);
			if (FAILED(r)) {
				SetError(_T("Unable to reacquire back buffer"));
				PostQuitMessage(0);
				return E_FAIL;
			}
			this->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
			this->RestoreGraphics();
		}
	}
	return S_OK;
}
HRESULT Engine::RestoreGraphics() {
	return S_OK;
}
bool Engine::LoadMesh(char * filename, int pos) {
	//temporary meshes
	ID3DXMesh* Mesh = nullptr;
	std::vector<D3DMATERIAL9> Mtrls(0);
	std::vector<IDirect3DTexture9*> Textures(0);

	// loading mesh
	HRESULT hr = 0;

	//
	// Load the XFile data.
	//

	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;

	hr = D3DXLoadMeshFromX(
		filename,
		D3DXMESH_MANAGED,
		this->pDevice,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if (FAILED(hr)) {
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if (mtrlBuffer != 0 && numMtrls != 0) {
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for (int i = 0; i < numMtrls; i++) {
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back(mtrls[i].MatD3D);


			// check if the ith material has an associative texture
			if (mtrls[i].pTextureFilename != 0) {
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					this->pDevice,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back(tex);
			}
			else {
				// no texture for the ith subset
				Textures.push_back(0);
			}
		}
	}
	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

											//
											// Optimize the mesh.
											//
	hr = Mesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if (FAILED(hr)) {
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}

	// Load more
	float ret[5];
	switch (pos) {
	case 0: // obj 1
		ret[0] = 0.0f;
		ret[1] = 0.0f;
		ret[2] = 0.0f;
		break;
	case 1: // obj 2
		ret[0] = 0.0f;
		ret[1] = 0.0f;
		ret[2] = 0.0f;
		break;
	case 2: // obj 3
		ret[0] = 0.0f;
		ret[1] = 0.0f;
		ret[2] = 0.0f;
		break;
	case 3: // obj 4
		ret[0] = 0.0f;
		ret[1] = 0.0f;
		ret[2] = 0.0f;
		break;
	case 4: // obj 5
		ret[0] = 0.0f;
		ret[1] = 0.0f;
		ret[2] = 0.0f;
		break;
	}
	
	MeshStruct retstruct(Mesh, Mtrls, Textures, { ret[0],ret[1],ret[2] });
	Meshes.push_back(retstruct);
}
bool Engine::RenderMesh(MeshStruct mesh) {
	D3DXMATRIX world;
	D3DXMatrixRotationY(&world, 50.0f);
	this->pDevice->SetTransform(D3DTS_WORLD, &world);
	D3DXMatrixTranslation(&world, mesh.pos.x, mesh.pos.y, mesh.pos.z);
	this->pDevice->SetTransform(D3DTS_WORLD, &world);
	for (int i = 0; i < mesh.mtrls.size(); i++) {
		this->pDevice->SetMaterial(&mesh.mtrls[i]);
		this->pDevice->SetTexture(0, mesh.tex[i]);
		mesh.mesh->DrawSubset(i);
	}
	return true;
}
void Engine::SetupMatrices()
{
	// For our world matrix, we will just leave it as the identity
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, 50.0f);
	pDevice->SetTransform(D3DTS_WORLD, &matWorld); // Takes whatever is in the matrix, and sets it to the world
												   // ** This is how we will move objects and rotate later **

												   // Set up our view matrix. A view matrix can be defined given an eye point,
												   // a point to lookat, and a direction for which way is up. Here, we set the
												   // eye five units back along the z-axis and up three units, look at the 
												   // origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -10.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&view, &vEyePt, &vLookatPt, &vUpVec);
	pDevice->SetTransform(D3DTS_VIEW, &view);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	float screenaspect = (float)DeviceWidth / (float)DeviceHeight;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, screenaspect, 1.0f, 500.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}
void Engine::updateCamera(float x, float y, float z) {
	D3DXMATRIX world;
	D3DXMatrixRotationY(&world, 50.0f);
	this->pDevice->SetTransform(D3DTS_WORLD, &world);
	D3DXMatrixTranslation(&world, x, y, z);
	this->pDevice->SetTransform(D3DTS_WORLD, &world);
}
/*
Sets the lighting to directional.
*/
void Engine::SetLightingDirectional() {
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;

	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	pDevice->SetMaterial(&material);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(10, 10, 10));
}
/*
Sets the lighting to ambient.
*/
void Engine::SetLightingAmbient() {
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));
}
/*
Sets the lighting to spot.
*/
void Engine::SetLightingSpot() {
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse.r = 1.0f;
	material.Diffuse.g = 1.0f;
	material.Diffuse.b = 1.0f;
	material.Diffuse.a = 1.0f;

	material.Ambient.r = 1.0f;
	material.Ambient.g = 1.0f;
	material.Ambient.b = 1.0f;
	material.Ambient.a = 1.0f;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_SPOT;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	light.Direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	light.Range = 100.0f;    // a range of 100
	light.Attenuation0 = 0.0f;    // no constant inverse attenuation
	light.Attenuation1 = 0.125f;    // only .125 inverse attenuation
	light.Attenuation2 = 0.0f;    // no square inverse attenuation
	light.Phi - D3DXToRadian(40.0f);
	light.Theta = D3DXToRadian(20.0f);
	light.Falloff = 1.0f;

	pDevice->SetMaterial(&material);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(10, 10, 10));
}
/*
Sets the lighting to point.
*/
void Engine::SetLightingPoint() {
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	light.Range = 100.0f;
	light.Attenuation0 = 0.0f;
	light.Attenuation1 = 0.125f;
	light.Attenuation2 = 0.0f;

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pDevice->SetMaterial(&material);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(10, 10, 10));
}
void Engine::setSelected(int i) {
	this->selected = i;
}