#pragma once
#include "Geometry/Rect.h"
class WayPoint :public Rect
{
	friend class GameManager;
public:
	
	WayPoint(D3DXVECTOR3 position, D3DXVECTOR3 size, wstring path = L"");
	~WayPoint();


	bool InWayPoint(RectEdges* other);
	wstring GetDestinationPath() { return destinationPath; }
	void SetDestinationPath(wstring destinationPath) { this->destinationPath = destinationPath; }


private:
	wstring destinationPath;
	wstring currentPath;
};