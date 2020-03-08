#include "stdafx.h"
#include "Program.h"
#include "Game/GameManager.h"

Program::Program()
{
	States::Create();
	GameManager::Create();
}

Program::~Program()
{
	GameManager::Delete();
}

void Program::Update()
{
	GameManager::Get()->Update();
}

void Program::PreRender()
{
	
}

void Program::Render()
{
	GameManager::Get()->Render();
}

void Program::GUI()
{
	GameManager::Get()->GUI();
}

void Program::PrintFrame()
{
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ WinMaxWidth - 200, 0 });
	if (ImGui::Begin("FrameRate", &bOpen, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
		string frameStr = "Frame Rate : " + to_string((int)ImGui::GetIO().Framerate);
		ImGui::Text(frameStr.c_str());
	}
	ImGui::End();
}
