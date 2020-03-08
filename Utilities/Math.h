#pragma once

class Math
{
public:
	static D3DXVECTOR2 AbsVec2(D3DXVECTOR2 val);
	static bool Approximation(D3DXVECTOR3 origin, D3DXVECTOR3 target, float approxValue);
	static bool Approximation(float origin, float target, float approxValue);
};