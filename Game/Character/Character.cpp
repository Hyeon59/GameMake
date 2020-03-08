#include "stdafx.h"
#include "Character.h"
#include "Geometry/CharacterRect.h"

Character::Character(D3DXVECTOR3 position, D3DXVECTOR3 size)
	: position(position), size(size)
{
	characterRect = new CharacterRect(position, size, 0);
}

Character::~Character()
{
	SAFE_DELETE(characterRect);
}

void Character::Update()
{
	characterRect->Update();
	position = characterRect->GetPosition();
	size = characterRect->GetSize();
}

void Character::Render()
{
	characterRect->Render();
}

void Character::Move(D3DXVECTOR3 newPos)
{
	characterRect->Move(newPos);
}

void Character::Reposition(D3DXVECTOR3 newPos)
{
	characterRect->Reposition(newPos);
}

void Character::Jump()
{
	
}

bool Character::Intersect(RectEdges* other)
{
	RectEdges* e1 = characterRect->GetEdges(); //this
	RectEdges* e2 = other; //충돌될 사각형

	if (e1->RB.x >= e2->LT.x
		&& e1->LT.x <= e2->RB.x
		&& e1->LT.y >= e2->RB.y
		&& e1->RB.y <= e2->LT.y)
		return true;
	else
		return false;
}

bool Character::Intersect(RectEdges * skill, RectEdges * other)
{
	RectEdges* e1 = skill; //this
	RectEdges* e2 = other; //충돌될 사각형

	if (e1->RB.x >= e2->LT.x
		&& e1->LT.x <= e2->RB.x
		&& e1->LT.y >= e2->RB.y
		&& e1->RB.y <= e2->LT.y)
		return true;
	else
		return false;
}

RectEdges* Character::GetEdge()
{
	return characterRect->GetEdges();
}
