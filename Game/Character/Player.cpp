#include "stdafx.h"
#include "Player.h"
#include "Geometry/Animation/Animator.h"
#include "Geometry/CharacterRect.h"
#include "Geometry/TextureRect.h"
#include "Geometry/Rect.h"

#define GUITEXT(name, value)	ImGui::Text(name); ImGui::SameLine(); ImGui::Text((to_string(value)).c_str())
#define SAME	ImGui::SameLine()
#define DELTA   Time::Get()->Delta()
#define Clip    characterRect->ChangeClip
#define player  characterRect


Player::Player(D3DXVECTOR3 position, D3DXVECTOR3 size)
	: Character(position, size)
{
	//Player
	playerHitBox = new Rect(position, size, 0);
	playerHitBox->ChangeColor(Value::Blue);
	playerAttackBox = new Rect(position, Value::Zero, 0);
	playerAttackBox->ChangeColor(Value::Yellow);

	//TestMap
	testMap = new TextureRect(Value::Zero, D3DXVECTOR3(2770, 970, 1));
	testMap->SetSRV(L"./_Textures/TestMap.png");
	//Way1 = new Rect({ testMap->GetEdges()->LT.x + 100, 190, 0 }, { 100, 100, 1 }, 0);
	//Way1->ChangeColor(Value::White);
	//Way2 = new Rect({ testMap->GetEdges()->RB.x - 150, 190, 0 }, { 100, 100, 1 }, 0);
	//Way2->ChangeColor(Value::White);

	//Info
	name = L"SGA";
	level = 1;
	exp = 0;
	hp = 100;
	mp = 100;
	physicalAttack = 10; // 물리공격력
	magicalAttack = 0; // 마법공격력
	physicalDefenseRate = 5.0f; // 물리방어력
	magicalDefenseRate = 5.0f; // 마법방어력
	hitRate = 0.0f; // 명중
	evasionRate = 0.0f; // 회피
	knockback = 0.0f; // 넉백
	money = 0;

	//이동, 점프
	speed = 150.0f;
	VX = 0.0f;
	VY = 0.0f;
	JumpSpeed = 500.0f;
	JumpAccel = -20.0;
	JumpCount = 0;
	Ground = position.y;
	MinX = testMap->GetPosition().x;
	MaxX = testMap->GetPosition().x + testMap->GetSize().x;

	//애니셋팅
	player->SetAnimation(L"./_Textures/Loco_L.json");
	player->AddAnimation(L"./_Textures/Loco_R.json");
	player->AddAnimation(L"./_Textures/Action.json");
	player->SetPlayRate(1.0f / 2.5f);
	player->ChangeClip(L"Stand_L");



}

Player::~Player()
{
	SAFE_DELETE(characterRect);
	SAFE_DELETE(testMap);
}

void Player::Update()
{
	Character::Update();
	UpdateHitBox();

	if (!bDead)
	{
		Move();
		Attack();
		Jump();
	}

	//if (Intersect(Way1->GetEdges()))
	//{
	//	if (KeyDown_Up)
	//	{
	//		player->Reposition(Way2->GetPosition());
	//		int a = 10;
	//	}
	//}
	//else if (Intersect(Way2->GetEdges()))
	//{
	//	if (KeyDown_Up)
	//	{
	//		player->Reposition(Way1->GetPosition());
	//		int a = 10;
	//	}
	//}

	Dead();
}

void Player::Render()
{
	//testMap->Render();
	//Way1->Render();
	//Way2->Render();

	if(!bAttack)
		playerHitBox->Render();
	playerAttackBox->Render();
	
	Character::Render();
}


void Player::Move()
{
	if (!bJump && !bAttack && !bBack )
	{
		VX = 0;

		if (bRight && bWalk)
		{
			Dir = 1;
			VX = speed * DELTA;
			Clip(L"Walk_R");
		}
		else if (bLeft && bWalk)
		{
			Dir = -1;
			VX = -speed * DELTA;
			Clip(L"Walk_L");
		}
	}

	if ( !bAlert && !bWalk && !bAttack && !bJump)
	{
		if (bLeft)
			Clip(L"Stand_L");
		else if (bRight)
			Clip(L"Stand_R");
	}

	player->Move(D3DXVECTOR3(VX, 0, 0));

	if (position.x < MinX)
		player->Reposition(D3DXVECTOR3(0, position.y, 0));
	if (player->GetEdges()->RB.x > MaxX)
		player->Reposition(D3DXVECTOR3(MaxX - size.x, position.y, 0));

}

void Player::Jump()
{
	int maxJump = 2;
	float double_VY = 7.0f;

	if (JumpCount == 0 && bJump)
	{
		JumpCount++;
		bAlert = false;

		if (bProne) // 하향점프, 나중에 구조물 체크 추가 ★★★★★
		{
			if (player->GetPosition().y > Ground)
				VY = (JumpSpeed/2) * DELTA;
		}
		else
			VY = JumpSpeed * DELTA;

	}
	else if ( JumpCount != 0 && bJump)
	{
		VY += JumpAccel * DELTA;
		player->Move(D3DXVECTOR3(VX, VY, 0));

		if (bLeft)
			Clip(L"Jump_L");
		else if (bRight)
			Clip(L"Jump_R");

		if (JumpCount < maxJump && bDJump && abs(VY) < double_VY)
		{
			if (bUp) // 윗점
			{
				VX = 0;
				VY = JumpSpeed * 1.3f * DELTA;
			}
			else if (!bRight || !bLeft) // 제자리 더블점프
			{
				VX = (float)Dir * speed * 2 * DELTA;
				VY = (JumpSpeed / 2) * DELTA;
			}
			else
			{
				VX = VX * 2 * DELTA;
				VY = (JumpSpeed / 2) * DELTA;
			}
			JumpCount++;
		}

		if (player->GetPosition().y <= Ground) // 바닥 착지
		{
			player->Reposition(D3DXVECTOR3(player->GetPosition().x, Ground, 0));
			ResetJump();
		}
	}
}

void Player::ResetJump()
{
	JumpCount = 0;
	bJump = false;
	bDJump = false;
	VX = 0;
	VY = 0;
}

void Player::Attack()
{
	if (bAttack)
	{
		BasicAttackSkill();
		player->SetLoop(false);

		playerAttackBox->Reposition({ position.x , position.y, 0 });
		playerAttackBox->Resize({ size.x, size.y, 1 });

		if (player->GetStop())
		{
			AlertON();
			player->SetLoop(true);
			player->SetStop(false);
			bAttack = false;
			playerAttackBox->Resize(Value::Zero);
		}
	}
	AlertOff();
}

void Player::Attacked(Character* monster)
{
	if (!bHit && !bDead)
	{
		bHit = true;
		hp -= monster->GetPAttack();
	}

	if (bHit && !bAttack && !bDead)
		AlertON();

	AlertOff();
}

void Player::AlertON()
{
	if (bLeft)
		Clip(L"Alert_L");
	else if (bRight)
		Clip(L"Alert_R");

	if (!bAlert)
	{
		if (!bBack && bHit && !bJump)
		{
			bBack = true;
			VX = -Dir * speed * 3 * DELTA;
			VY = JumpSpeed * DELTA;
		}
	}
	if (bBack)
	{
		VY += JumpAccel * DELTA;
		Character::Move({ VX, VY, 0 });

		if (position.y <= Ground) // 바닥 착지
		{
			player->Reposition(D3DXVECTOR3(player->GetPosition().x, Ground, 0));
			ResetJump();
			bBack = false;
		}
	}

	bAlert = true;
}

void Player::AlertOff()
{
	if ((bAlert || bHit) && !bDead)
	{
		if (delta > duration)
		{
			bAlert = false;
			bHit = false;
			delta = 0;
		}
		else
			delta += DELTA;
	}
}

void Player::Dead()
{
	if (hp <= 0 && !bDead)
	{
		if (bLeft)
			player->ChangeClip(L"Dead_L");
		else if (bRight)
			player->ChangeClip(L"Dead_R");

		bDead = true;

		if (bHit)
		{
			bHit = false;
			exp -= 10;
		}
	}

	if (!bHit && bDead)
	{
		if (VY != 0)
		{
			VY += JumpAccel * DELTA;
			player->Move({ 0, VY, 0 });

			if (player->GetPosition().y <= Ground) // 바닥 착지
			{
				player->Reposition(D3DXVECTOR3(player->GetPosition().x, Ground, 0));
				ResetJump();
			}
		}

		if (KeyDown_Space)
			Respawn();
	}

}

void Player::Respawn()
{
	player->ChangeClip(L"Stand_L");
	Reposition({ WinMaxWidth / 2 + 300 ,190, 0 });
	bDead = false;
	hp = 50;
	mp = 50;
}

void Player::BasicAttackSkill()
{
	if (type == 0)
	{
		if (bLeft)
			Clip(L"StabO1_L");
		else if (bRight)
			Clip(L"StabO1_R");
	}
	else if (type == 1)
	{
		if (bLeft)
			Clip(L"StabO2_L");
		else if (bRight)
			Clip(L"StabO2_R");
	}
	else if (type == 2)
	{
		if (bLeft)
			Clip(L"StabT1_L");
		else if (bRight)
			Clip(L"StabT1_R");
	}
	else if (type == 3)
	{
		if (bLeft)
			Clip(L"StabT2_L");
		else if (bRight)
			Clip(L"StabT2_R");
	}
}

RectEdges * Player::AttackBoxEdges()
{
	return playerAttackBox->GetEdges();
}

void Player::UpdateHitBox()
{
	playerHitBox->Reposition(position);

	if (bHit)
		playerHitBox->ChangeColor(Value::Red);
	else
		playerHitBox->ChangeColor(Value::Blue);
}



RectEdges  Player::GetMapEdge()
{
	RectEdges temp;
	temp.RB.x = testMap->GetEdges()->RB.x;
	temp.RB.y = testMap->GetEdges()->RB.y;
	temp.LT.x = testMap->GetEdges()->LT.x;
	temp.LT.y = testMap->GetEdges()->LT.y;
	return temp;
}

void Player::GUI()
{
	static bool bOpen = true;
	ImGui::Begin("Player", &bOpen, ImGuiWindowFlags_NoBackground);
	{
		string text = "메이플";
		ImGui::TextColored({ 1,1,0,1 }, text.c_str());

		ImGui::Text("NAME : "); SAME; ImGui::Text(String::ToString(name).c_str());
		GUITEXT("LEVEL : ", level);
		GUITEXT("HP : ", (int)hp);
		GUITEXT("MP : ", (int)mp );
		GUITEXT("EXP : ", (int)exp);
		GUITEXT("MONEY : ", money);
		ImGui::Separator();
		GUITEXT("SPEED : ", (int)speed);
		GUITEXT("VX : ", (int)VX);
		GUITEXT("VY : ", (int)VY);
		GUITEXT("DIR : ", Dir);
		GUITEXT("JUMP SPEED : ", (int)JumpSpeed);
		GUITEXT("JUMP ACCEL : ", (int)JumpAccel);
		GUITEXT("JUMP COUNT : ", JumpCount);	
		
	}
	ImGui::End();

	ImGui::Begin("Bool", &bOpen, ImGuiWindowFlags_NoBackground);
	{
		GUITEXT("ATTACK : ", bAttack);
		GUITEXT("ALERT : ", bAlert);
		GUITEXT("HIT : ", bHit);
		GUITEXT("BACK : ", bBack);
		ImGui::Separator();
		GUITEXT("WALK : ", bWalk);
		GUITEXT("JUMP : ", bJump);
		GUITEXT("DJUMP : ", bDJump);
		GUITEXT("PRONE : ", bProne);
		GUITEXT("LOPE : ", bLope);
		GUITEXT("SIT : ", bSit);
		ImGui::Separator();
		GUITEXT("UP : ", bUp);
		GUITEXT("RIGHT : ", bRight);
		GUITEXT("LEFT : ", bLeft);
		GUITEXT("DEAD : ", bDead);

		GUITEXT("Stop : ", characterRect->GetStop());
		GUITEXT("Loop : ", characterRect->GetLoop());
	}
	ImGui::End();

	ImGui::Begin("Rect", &bOpen, ImGuiWindowFlags_NoBackground);
	{
		GUITEXT("X : ", (int)position.x); SAME; GUITEXT("  Y : ", (int)position.y);
		GUITEXT("LT [", (int)player->GetEdges()->LT.x); SAME; GUITEXT("][", (int)player->GetEdges()->LT.y); SAME; ImGui::Text("]");
		GUITEXT("RB [", (int)player->GetEdges()->RB.x); SAME; GUITEXT("][", (int)player->GetEdges()->RB.y); SAME; ImGui::Text("]");
	}
	ImGui::End();

	ImGui::Begin("PLAYER GUID", &bOpen);
	{
		ImGui::Text("MOVE : <-  ->");
		ImGui::Text("JUMP : X / X + X / UP + X ");
		ImGui::Text("Attack : Ctrl");
		ImGui::Text("Respawn : Space");
		//ImGui::Text("Skill : A, S, D");
	}
	ImGui::End();
}


