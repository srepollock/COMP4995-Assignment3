#pragma once
#include "DirectX.h"
#include "basics.h"
#ifndef BOUNDING_BOX
#define BOUNDING_BOX
class BoundingBox {
public:
	BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};
#endif