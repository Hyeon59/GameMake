#include "stdafx.h"
#include "Rect.h"

UINT Rect::objectIndex = 0;

Rect::Rect()
{
	index = objectIndex++;
}

Rect::Rect(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation)
	: position(position), size(size), rotation(rotation)
{
	vertices.assign(4, VertexColor());

	vertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertices[0].color = Value::Red;

	vertices[1].position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertices[1].color = Value::Red;

	vertices[2].position = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vertices[2].color = Value::Red;

	vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertices[3].color = Value::Red;

	VB = new VertexBuffer();
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	indices = { 0,1,2,2,1,3 };

	IB = new IndexBuffer();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(L"./_Shaders/Color.hlsl", "VS");

	PS = new PixelShader();
	PS->Create(L"./_Shaders/Color.hlsl", "PS");

	IL = new InputLayout();
	IL->Create(VertexColor::descs, VertexColor::count, VS->GetBlob());

	WB = new WorldBuffer();

	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	D3DXMatrixScaling(&S, size.x, size.y, size.z);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));

	world = S * R * T;

	WB->SetWorld(world);
	TransformVertices();

	index = objectIndex++;
}

Rect::~Rect()
{
	SAFE_DELETE(WB);

	SAFE_DELETE(IL);

	SAFE_DELETE(PS);
	SAFE_DELETE(VS);

	SAFE_DELETE(IB);
	SAFE_DELETE(VB);
}

void Rect::Initialize(vector<VertexColor>& vertices, vector<UINT>& indices, D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation, wstring shaderPath)
{
	this->vertices.assign(vertices.begin(), vertices.end());
	this->indices.assign(indices.begin(), indices.end());
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	this->shaderPath = shaderPath;

	VB = new VertexBuffer();
	VB->Create(this->vertices, D3D11_USAGE_DYNAMIC);

	IB = new IndexBuffer();
	IB->Create(this->indices, D3D11_USAGE_IMMUTABLE);

	VS = new VertexShader();
	VS->Create(this->shaderPath, "VS");

	PS = new PixelShader();
	PS->Create(this->shaderPath, "PS");

	IL = new InputLayout();
	IL->Create(VertexColor::descs, VertexColor::count, VS->GetBlob());

	WB = new WorldBuffer();

	D3DXMatrixTranslation(&T, this->position.x, this->position.y, this->position.z);
	D3DXMatrixScaling(&S, this->size.x, this->size.y, this->size.z);
	D3DXMatrixRotationZ(&R, (float)D3DXToRadian(this->rotation));

	world = S * R * T;

	WB->SetWorld(world);
	TransformVertices();

	//index = objectIndex++;
}

void Rect::TransformVertices()
{
	D3DXVec3TransformCoord(&edge.LT, &vertices[1].position, &world);
	D3DXVec3TransformCoord(&edge.RB, &vertices[2].position, &world);
}

void Rect::Update()
{
	if (bJump)
	{
		vy += jumpAccel * Time::Delta();
		position.y += vy * Time::Delta();
		if (vy < 0 && position.y <= 100)
		{
			bJump = false;
			position.y = 100.0f;
		}
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);
		world = S * R * T;
		WB->SetWorld(world);
		TransformVertices();
	}
}

void Rect::Render()
{
	VB->SetIA();
	IB->SetIA();

	IL->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	VS->SetShader();
	WB->SetVSBuffer(0);

	PS->SetShader();

	DC->DrawIndexed(IB->GetCount(), 0, 0);
}

void Rect::Move(D3DXVECTOR3 position)
{
	this->position = position;
	D3DXMatrixTranslation(&T, this->position.x, this->position.y, this->position.z);
	world = S * R* T;
	WB->SetWorld(world);
	TransformVertices();
}

bool Rect::Intersect(Rect * other)
{
	RectEdges e1 = this->edge; //this
	RectEdges e2 = *other->GetEdges(); //충돌될 사각형

	if (e1.RB.x >= e2.LT.x
		&& e1.LT.x <= e2.RB.x
		&& e1.LT.y >= e2.RB.y
		&& e1.RB.y <= e2.LT.y)
		return true;
	else
		return false;
}

void Rect::Reposition(D3DXVECTOR3 newPos)
{
	this->position = newPos;
	D3DXMatrixTranslation(&T, this->position.x, this->position.y, this->position.z);
	world = S * R * T;
	WB->SetWorld(world);
	TransformVertices();
}

void Rect::Resize(D3DXVECTOR3 size)
{
	this->size = size;
	D3DXMatrixScaling(&S, this->size.x, this->size.y, this->size.z);
	world = S * R * T;
	WB->SetWorld(world);
	TransformVertices();
}

bool Rect::CheckEnd()
{
	if (edge.RB.x < -100)
		return true;
	else 
		return false;
}

void Rect::GUI()
{
	static bool bOpen = true;
	string label = "Rect" + to_string(index);
	if (ImGui::BeginMenu(label.c_str(), &bOpen))
	{
		ChangeColor();

		string posX = "X : " + to_string(position.x);
		string posY = "Y : " + to_string(position.y);
		string posZ = "Z : " + to_string(position.z);

		ImGui::Text(posX.c_str());
		ImGui::Text(posY.c_str());
		ImGui::Text(posZ.c_str());

		if (ImGui::InputFloat3("Position", position, 2, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			D3DXMatrixTranslation(&T, position.x, position.y, position.z);
			world = S * R * T;
			WB->SetWorld(world);
		}

		ImGui::EndMenu();
	}
		
}

void Rect::ChangeColor()
{
	if (ImGui::ColorPicker4("ColorPicker", color))
	{
		D3D11_MAPPED_SUBRESOURCE subResource;

		DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			for (VertexColor& v : vertices)
				v.color = color;

			memcpy(subResource.pData, vertices.data(), sizeof(VertexColor) * vertices.size());
		}
		DC->Unmap(VB->GetResource(), 0);
	}
}

void Rect::ChangeColor(D3DXCOLOR color)
{
	D3D11_MAPPED_SUBRESOURCE subResource;

	DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		for (VertexColor& v : vertices)
			v.color = color;

		memcpy(subResource.pData, vertices.data(), sizeof(VertexColor) * vertices.size());
	}
	DC->Unmap(VB->GetResource(), 0);
}

void Rect::Jump()
{
	bJump = true;
	vy = jumpSpeed;
}
