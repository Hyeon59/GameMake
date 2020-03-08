#include "stdafx.h"
#include "BaseMap.h"
#include "Geometry/Rect.h"
#include "Map/WayPoint.h"


BaseMap::BaseMap(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation)
	:position(position),size(size),rotation(rotation)
{
	vertices.assign(4, VertexTexture());

	vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[0].uv = D3DXVECTOR2(0, 1);

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[1].uv = D3DXVECTOR2(0, 0);

	vertices[2].position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[2].uv = D3DXVECTOR2(1, 1);

	vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[3].uv = D3DXVECTOR2(1, 0);

	VB = new VertexBuffer();
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	indices = { 0,1,2,2,1,3 };

	IB = new IndexBuffer();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(L"./_Shaders/Texture.hlsl", "VS");

	PS = new PixelShader();
	PS->Create(L"./_Shaders/Texture.hlsl", "PS");

	IL = new InputLayout();
	IL->Create(VertexTexture::descs, VertexTexture::count, VS->GetBlob());

	WB = new WorldBuffer();

	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	D3DXMatrixScaling(&S, size.x, size.y, size.z);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));

	world = S * R * T;

	WB->SetWorld(world);
	TransformVertices();

	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &bsState_disabled);

		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &bsState_enabled);
	}
	monsterMaxNum = 0;
	monsterGenCycle = .0f;
	monsterName = "mon1";
	//destinationPath = L"./_Textures/TestMap2.png";

	
}

BaseMap::~BaseMap()
{
	SAFE_DELETE(WB);

	SAFE_DELETE(IL);

	SAFE_DELETE(PS);
	SAFE_DELETE(VS);

	SAFE_DELETE(IB);
	SAFE_DELETE(VB);

	
	
}

void BaseMap::TransformVertices()
{
	D3DXVec3TransformCoord(&edge.LT, &vertices[1].position, &world);
	D3DXVec3TransformCoord(&edge.RB, &vertices[2].position, &world);
}

void BaseMap::Update()
{
	
	if (Mouse::Get()->Down(0))
	{
		MakeGroundTool(GetMousePos(), groundSize, groundRotation);
		groundCount++;
	}
	if (Mouse::Get()->Down(1))
	{
		if (grounds.size() != 0)
		{
			grounds.pop_back();
			groundCount--;
		}
	}

	if (Keyboard::Get()->Press(VK_LSHIFT))
	{
		if (Mouse::Get()->Down(0))
		{
			lWayPoint = new WayPoint(GetMousePos(), D3DXVECTOR3(200, 200, 0), L"./_Maps/1.data");
		}
		else if(Mouse::Get()->Down(1))
		{
			rWayPoint = new WayPoint(GetMousePos(), D3DXVECTOR3(200, 200, 0), L"./_Maps/3.data");
		}
	}
}

void BaseMap::Render()
{
	VB->SetIA();
	IB->SetIA();

	IL->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VS->SetShader();
	WB->SetVSBuffer(0);

	PS->SetShader();
	if (srv)
		DC->PSSetShaderResources(5, 1, &srv);

	DC->OMSetBlendState(bsState_enabled, NULL, 0xffffffff);
	DC->DrawIndexed(IB->GetCount(), 0, 0);
	//DC->OMSetBlendState(bsState_disabled, NULL, 0xffffffff);
	DC->PSSetShaderResources(0, 1, &nullView);

	if (grounds.size() != 0)
	{
		for (auto ground : grounds)
		{
			ground->Render();
		}
	}

	if (lWayPoint)
	{
		lWayPoint->Render();
	}
	if (rWayPoint)
	{
		rWayPoint->Render();
	}
}

bool BaseMap::CheckEnd()
{
	if (edge.RB.x < -100)
		return true;
	else
		return false;
}

void BaseMap::Reposition(D3DXVECTOR3 newPos)
{
	this->position = newPos;
	D3DXMatrixTranslation(&T, this->position.x, this->position.y, this->position.z);
	world = S * R * T;
	WB->SetWorld(world);
	TransformVertices();
}

void BaseMap::SetSRV(wstring path)
{
	this->path = path;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		DEVICE,
		path.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	);
	ASSERT(hr);
}

D3DXVECTOR3 BaseMap::GetMousePos()
{
	D3DXVECTOR3 mPos = Mouse::Get()->GetPosition();
	Camera::Get()->UnProject(&worldPos, mPos, Value::Identity);
	return worldPos;
	
}

void BaseMap::SetGround(Rect* ground)
{
	grounds.push_back(ground);
}

void BaseMap::MakeGroundTool(D3DXVECTOR3 mousePos,D3DXVECTOR3 groundSize,float groundRotation)
{
	grounds.push_back(new Rect(mousePos, groundSize,groundRotation));
}

void BaseMap::GUI()


{
	ImGui::Begin("MapIncludeInformation");
	{

		ImGui::InputInt("MonsterMaxNum", &monsterMaxNum);
		ImGui::InputFloat("MonsterGenCycle", &monsterGenCycle, 1.0f, 100.0f, "%.1f");
		ImGui::InputFloat("GroundSize.x", &groundSize.x, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat("GroundSize.y", &groundSize.y, 1.0f, 1000.0f, "%.1f");
		ImGui::InputFloat("GroundRotation", &groundRotation, 1.0f, 359.0f, "%.1f");

		ImGui::Text("MonsterMaxNum : %d", monsterMaxNum);
		ImGui::Text("MonsterGenCycle : %f", monsterGenCycle);

		ImGui::End();
	}

}
