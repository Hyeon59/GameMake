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

	// �̵�
	void Move();
	void Jump();
	void ResetJump();

	// ����
	void Attack();
	void Attacked(Character* monster);
	void AlertON();
	void AlertOff();
	void Dead();
	void Respawn();

	// ��ų
	void BasicAttackSkill(); // �Ϲݰ���
	int type = 0;

	RectEdges* AttackBoxEdges();


private:
	//����
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

	//�̵�
	bool bWalk = false;
	bool bUp = false;
	bool bRight = false;
	bool bLeft = true;
	int Dir = -1;			// 1 ������ -1 ����

	//��
	class TextureRect* testMap;
	RectEdges GetMapEdge();
	float Ground;
	float MinX, MaxX;

	//class Rect* Way1;
	//class Rect* Way2;

	//Alert �ð�üũ
	float delta = 0.0f;
	float duration = 3.0f;

	//����
	class Rect* playerHitBox;
	class Rect* playerAttackBox;
	void UpdateHitBox();

	
};
	