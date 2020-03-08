#pragma once

class Monster : public Character
{
public:
	Monster(D3DXVECTOR3 position, D3DXVECTOR3 size);
	~Monster();

	void Update();
	void Render();
	void GUI();

	void MoveWithoutAggro(RectEdges* land);
	void MoveWithAggro(RectEdges* land, D3DXVECTOR3 playerPosition);
	void MonsterBHit(UINT level, float physicalAttack, float magicalAttack, float hitRate);
	void Evade();

	UINT GetDirection() { return direction; }
	bool GetMonsterAggroState() { return bAggro; }
private:
	UINT direction = 0;
	bool bHit = false;
	bool bAggro = false;
	D3DXVECTOR3 unprojectedPos = Value::Zero;
};