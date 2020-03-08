#include "stdafx.h"
#include "WayPoint.h"
#include "Game/GameManager.h"

WayPoint::WayPoint(D3DXVECTOR3 position, D3DXVECTOR3 size, wstring path)
	: Rect(position, size, 0.0f),destinationPath(path)
{
	
}

WayPoint::~WayPoint()
{
	
}

bool WayPoint::InWayPoint(RectEdges * other)
{
	RectEdges* e1 = this->GetEdges();
	RectEdges* e2 = other;
	if (e1->LT.x < e2->LT.x &&
		e1->LT.y>e2->LT.y&&
		e1->RB.x > e2->RB.x &&
		e1->RB.y < e2->RB.y)
	{	
		return true;
	}
	else
		return false;

		//this->GetEdges()->LT.x < other->LT.x &&
		//this->GetEdges()->LT.y>other->LT.y&&
		//this->GetEdges()->RB.x > other->RB.x &&
		//this->GetEdges()->RB.y < other->RB.y

	/*e1->LT.x < e2->LT.x &&
		e1->LT.y>e2->LT.y&&
		e1->RB.x > e2->RB.x &&
		e1->RB.y < e2->RB.y*/

}
