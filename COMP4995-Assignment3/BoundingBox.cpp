#include "BoundingBox.h"
BoundingBox::BoundingBox() {
	// infinite small 
	_min.x = BINFINITY;
	_min.y = BINFINITY;
	_min.z = BINFINITY;

	_max.x = -BINFINITY;
	_max.y = -BINFINITY;
	_max.z = -BINFINITY;
}

bool BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if (p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}