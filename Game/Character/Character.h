#pragma once

class CharacterRect;

class Character
{
public:
	Character(D3DXVECTOR3 position, D3DXVECTOR3 size);
	~Character();

	void Update();
	void Render();

	void Move(D3DXVECTOR3 newPos);
	void Reposition(D3DXVECTOR3 newPos);
	void Jump();

	bool Intersect(RectEdges* other);
	bool Intersect(RectEdges* skill, RectEdges* other);
	D3DXVECTOR3 GetPosition() { return position; }
	RectEdges* GetEdge();

	float GetPAttack() { return physicalAttack; }

protected:
	wstring name = L"";
	UINT level = 1;
	int exp = 0;
	float hp = 0.0f;
	float mp = 0.0f;
	float physicalAttack = 0.0f;
	float magicalAttack = 0.0f;
	float physicalDefenseRate = 0.0f;
	float magicalDefenseRate = 0.0f;
	float hitRate = 0.0f;
	float evasionRate = 0.0f;
	float knockback = 0.0f;
	int money = 0;

	// มกวม
	float speed = 2.0f;
	float VX = 0.0f;
	float VY = 0.0f;
	float JumpSpeed = 5.0f;
	float JumpAccel = -0.5f;
	int JumpCount = 0;

	CharacterRect* characterRect;
	
	D3DXVECTOR3 position;
	D3DXVECTOR3 size;
};