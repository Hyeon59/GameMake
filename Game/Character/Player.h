#pragma once
#include "Character.h"

class Player : public Character
{
	friend class GameManager;

public:
	Player(D3DXVECTOR3 position, D3DXVECTOR3 size = D3DXVECTOR3(82, 71, 1));
	~Player();

	void Update();
	void Render();
	void GUI();

	// 이동
	void Move();
	void Jump();
	void ResetJump();

	// 전투
	void Attack();
	void Attacked(Character* monster);
	void AlertON();
	void AlertOff();
	void Dead();
	void Respawn();

	// 스킬
	void BasicAttackSkill(); // 일반공격
	int type = 0;

	RectEdges* AttackBoxEdges();


private:
	//상태
	bool bAttack = false;
	bool bHit = false;
	bool bAlert = false;
	bool bJump = false;
	bool bDJump = false;
	bool bProne = false;
	bool bLope = false;
	bool bSit = false;
	bool bDead = false;
	bool bBack = false;

	//이동
	bool bWalk = false;
	bool bUp = false;
	bool bRight = false;
	bool bLeft = true;
	int Dir = -1;			// 1 오른쪽 -1 왼쪽

	//맵
	class TextureRect* testMap;
	RectEdges GetMapEdge();
	float Ground;
	float MinX, MaxX;

	//class Rect* Way1;
	//class Rect* Way2;

	//Alert 시간체크
	float delta = 0.0f;
	float duration = 3.0f;

	//전투
	class Rect* playerHitBox;
	class Rect* playerAttackBox;
	void UpdateHitBox();

	
};
	