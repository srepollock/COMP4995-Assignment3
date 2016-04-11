#pragma once
#include "DirectX.h"
#ifndef MESH
#define MESH
class Mesh {
public:
	ID3DXMesh* mesh;
	std::vector<D3DMATERIAL9> mtrls;
	std::vector<IDirect3DTexture9*> tex;
	D3DXVECTOR3 pos;
	Mesh(ID3DXMesh* m, std::vector<D3DMATERIAL9> mtrl, std::vector<IDirect3DTexture9*> t, D3DXVECTOR3 p) {
		mesh = m;
		mtrls = mtrl;
		tex = t;
		pos = p;
	}
};
#endif