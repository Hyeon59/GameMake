#include "stdafx.h"
#include "Geometry/Animation/Animator.h"
#include "Character.h"
#include "Geometry/CharacterRect.h"
#include "Monster.h"

Monster::Monster(D3DXVECTOR3 position, D3DXVECTOR3 size)
	: Character(position, size)
{
	name = L"파란 달팽이";
	level = 2;
	exp = 4;
	hp = 20.0f;
	mp = 0.0f;
	physicalAttack = 3.0f;
	magicalAttack = 2.0f;
	physicalDefenseRate = 0.1f;
	magicalDefenseRate = 0.1f;
	hitRate = 10.0f;
	evasionRate = 0.0f;
	knockback = 5.0f;
	money = 10;

	characterRect->SetAnimation(L"./_Textures/BlueSnail.json");
	characterRect->SetPlayRate(1.0f / 2.0f);
	characterRect->ChangeClip(L"Stand_L");

	int a = 10;
}

Monster::~Monster()
{
}

void Monster::Update()
{
	Character::Update();
}

void Monster::Render()
{
	Character::Render();
}

void Monster::GUI()
{
	Camera::Get()->UnProject(&unprojectedPos, position, Value::Identity);
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ unprojectedPos.x + size.x / 2.0f + 10.0f - WinMaxWidth / 2, unprojectedPos.y + size.y - 10.0f + WinMaxHeight / 2 });
	if (ImGui::Begin("FrameRate", &bOpen, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), "HP : "); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), (to_string((int)hp)).c_str());
	}
	ImGui::End();
}

void Monster::MoveWithoutAggro(RectEdges* land)
{
	if ((int)position.x + (int)size.x / 2 <= (int)land->LT.x)
		direction = 0;
	else if ((int)position.x + (int)size.x / 2 >= (int)land->RB.x)
		direction = 1;
	if (direction == 0)
		Move({ position.x + 2.0f, land->LT.y, position.z });
	else if (direction == 1)
		Move({ position.x - 2.0f, land->LT.y, position.z });
}

void Monster::MoveWithAggro(RectEdges* land, D3DXVECTOR3 playerPosition)
{
	if ((int)position.x <= (int)playerPosition.x)
		direction = 0;
	else if ((int)position.x >= (int)playerPosition.x)
		direction = 1;
	if (direction == 0)
		Move({ position.x + 2.0f, land->LT.y, position.z });
	else if (direction == 1)
		Move({ position.x - 2.0f, land->LT.y, position.z });
}

void Monster::MonsterBHit(UINT level, float physicalAttack, float magicalAttack, float hitRate)
{
	float requiredHitRate;
	if (this->level > level)
		requiredHitRate = (55 + (this->level - level) * 2) * evasionRate / 15;
	else
		requiredHitRate = 55 * evasionRate / 15;

	if (hitRate >= requiredHitRate)
	{
		hp -= physicalAttack * (1 - physicalDefenseRate) + magicalAttack * (1 - magicalDefenseRate);
		if (direction == 0)
			Move({ position.x - knockback, position.y, position.z });
		else
			Move({ position.x + knockback, position.y, position.z });
		bAggro = true;
	}
	else if ((requiredHitRate - hitRate) >= rand() / (float)RAND_MAX * 100.0f)
	{
		Evade();
		return;
	}
}

void Monster::Evade()
{
	//회피 애니메이션만 출력
}
