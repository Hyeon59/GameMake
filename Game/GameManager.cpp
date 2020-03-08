#include "stdafx.h"
#include "GameManager.h"
#include "Game/Character/Character.h"
#include "Game/Character/Player.h"
#include "Game/Character/Monster.h"
#include "Geometry/Rect.h"
#include "Map/BaseMap.h"
#include "Utilities/FileStream.h"
#include "Map/WayPoint.h"

void GameManager::Update()
{
	monster->Update();
	if (monster->GetMonsterAggroState())
		monster->MoveWithAggro(testMap->GetEdges(), player->GetPosition());
	else
		monster->MoveWithoutAggro(testMap->GetEdges());
	
	player->Update(); // HYEON
	HYEON(); // HYEON

	

	//주원=======================================


	//if (KeyPress_Right)
	//	testCharacter->Move({ testCharacter->GetPosition().x + 10.0f, testCharacter->GetPosition().y, testCharacter->GetPosition().z });
	//else if (KeyPress_Left)
	//	testCharacter->Move({ testCharacter->GetPosition().x - 10.0f, testCharacter->GetPosition().y, testCharacter->GetPosition().z });
	//if (KeyPress_Up)
	//	testCharacter->Move({ testCharacter->GetPosition().x , testCharacter->GetPosition().y + 10.0f, testCharacter->GetPosition().z });
	//else if (KeyPress_Down)
	//	testCharacter->Move({ testCharacter->GetPosition().x, testCharacter->GetPosition().y - 10.0f, testCharacter->GetPosition().z });



	if (testMap->GetWayPointL() != nullptr
		&& (testMap->GetWayPointL()->InWayPoint(player->GetEdge()))
		&& Keyboard::Get()->Down('W'))
	{
		LoadGameMap(testMap->GetWayPointL()->GetDestinationPath());
		player->Reposition(testMap->GetWayPointR()->GetPosition());

		//Camera::Get()->SetTileMapBoundary(testMap->GetEdges());		
		//Camera::Get()->SetPosition({ testMap->GetSize().x / 2, 0.0f, 0.0f });
	}
	if (testMap->GetWayPointR() != nullptr
		&& (testMap->GetWayPointR()->InWayPoint(player->GetEdge()))
		&& Keyboard::Get()->Down('W'))
	{
		LoadGameMap(testMap->GetWayPointR()->GetDestinationPath());
		player->Reposition(testMap->GetWayPointL()->GetPosition() + D3DXVECTOR3(75.0f, 0, 0));

		//Camera::Get()->SetTileMapBoundary(testMap->GetEdges());
		//Camera::Get()->SetPosition({ 0.0f,0.0f,0.0f });
	}




	testMap->Update();
	


	Camera::Get()->Update();
	Camera::Get()->SetPosition(player->GetPosition());


	//=========================================
	
}

void GameManager::Render()
{
	Camera::Get()->Render();

	testMap->Render();
	monster->Render();

	player->Render(); // HYEON
}

void GameManager::GUI()
{
	monster->GUI();
	//주원=-========================

	ImGui::Begin("MapSaveLoadTest");
	{
		if (ImGui::Button("Save", ImVec2(50, 30)))
			SaveGameMap();

		ImGui::SameLine();

		if (ImGui::Button("Load", ImVec2(50, 30)))
			LoadGameMap();

	}
	ImGui::End();

	ImGui::Begin("Camera Pos,CharacterPos");
	{
		ImGui::Text("XPos : %f", Camera::Get()->GetPosition().x);
		ImGui::Text("YPos : %f", Camera::Get()->GetPosition().y);

		ImGui::Text("ch X Pos : %f",player->GetPosition().x);
		ImGui::Text("ch Y Pos : %f",player->GetPosition().y);

		ImGui::Text("MapSize width : %f", testMap->GetSize().x);
		ImGui::Text("MapSize height : %f", testMap->GetSize().y);
	}
	ImGui::End();

	testMap->GUI();
	player->GUI();
	//=============================
}

void GameManager::HYEON()
{
	// 이동
	if (KeyPress_Right)
	{
		player->bRight = true; 
		player->bWalk = true;
		player->bLeft = false;
	}
	else if (KeyUp_Right)
		player->bWalk = false;

	if (KeyDown_Right)
	{
		player->bRight = true; 
		player->bLeft = false;
	}
	else if (KeyDown_Left)
	{
		player->bLeft = true;
		player->bRight = false;
	}

	if (KeyPress_Left)
	{
		player->bLeft = true;
		player->bWalk = true;
		player->bRight = false;
	}
	else if (KeyUp_Left)
		player->bWalk = false;

	if (KeyPress_Down)
		player->bProne = true;
	else if (KeyUp_Down)
		player->bProne = false;

	// 점프
	if (KeyDown_X && player->bJump)
		player->bDJump = true;
	if (KeyDown_X)
		player->bJump = true;

	// 공격
	if (KeyDown_Ctrl)
		player->bAttack = true;

}

void GameManager::SaveGameMap(const wstring & path)
{
	if (path.length() < 1)
	{
		function<void(wstring)> func = bind(&GameManager::SaveGameMap, this, placeholders::_1);
		Path::SaveFileDialog(L"", Path::TileDataFilter, L"./", func, handle);

	}
	else
	{
		FileStream* w = new FileStream(String::ToString(path), FILE_STREAM_WRITE);
		w->Write(testMap);
		//wstring tempPath = L"./_TileMap/" + Path::GetFileName(path);
		//testMap->SetCurrentMapPath(path);

		SAFE_DELETE(w);
	}

}

void GameManager::LoadGameMap(const wstring & path)
{
	if (path.length() < 1)
	{
		function<void(wstring)> func = bind(&GameManager::LoadGameMap, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::TileDataFilter, L"./", func, handle);

	}
	else
	{
		FileStream* r = new FileStream(String::ToString(path), FILE_STREAM_READ);

		tempMap = r->ReadBaseMap();

		if (tempMap)
		{
			SAFE_DELETE(testMap);
			testMap = tempMap;
			tempMap = nullptr;
		}
		SAFE_DELETE(r);
	}
}

GameManager::GameManager()
{
	Camera::Create();

	//land = new Rect({ WinMaxWidth / 2 , 0, 0 }, D3DXVECTOR3(500, 190, 1), 0);
	testMap = new BaseMap(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(2770, 970, 1));
	testMap->SetSRV(L"./_Textures/TestMap3.png");
	
	monster = new Monster({ testMap->GetEdges()->LT.x + (testMap->GetEdges()->RB.x - testMap->GetEdges()->LT.x) / 2 , testMap->GetEdges()->LT.y, 0 }, D3DXVECTOR3(100, 100, 1));
	player = new Player({ WinMaxWidth / 2 + 300 ,190, 0 });  // HYEON
	
	Camera::Get()->SetPosition(player->position); // HYEON
	Camera::Get()->SetTileMapBoundary(&player->GetMapEdge()); // HYEON

	//주원=-========================
	



	//=============================
}

GameManager::~GameManager()
{
	//SAFE_DELETE(land);
	SAFE_DELETE(monster);

	SAFE_DELETE(player); // HYEON
	Camera::Delete();

	//주원=-========================
	SAFE_DELETE(testMap);


	//=============================
}
