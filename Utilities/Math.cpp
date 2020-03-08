#include "stdafx.h"
#include "Math.h"

D3DXVECTOR2 Math::AbsVec2(D3DXVECTOR2 val)
{
	float x = fabs(val.x);
	float y = fabs(val.y);

	return D3DXVECTOR2(x, y);
}

bool Math::Approximation(D3DXVECTOR3 origin, D3DXVECTOR3 target, float approxValue)
{
	bool result = true;
	result &= Approximation(origin.x, target.x, approxValue);
	result &= Approximation(origin.y, target.y, approxValue);
	result &= Approximation(origin.z, target.z, approxValue);

	return result;
}

bool Math::Approximation(float origin, float target, float approxValue)
{
	bool result = abs(origin - target) <= approxValue;
	return result;
}
