#pragma once
#include "DirectX.h"
#ifndef RAY
#define RAY
class Ray {
public:
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};
#endif