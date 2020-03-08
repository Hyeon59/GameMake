#include "stdafx.h"
#include "FileStream.h"
#include "Geometry/Rect.h"
#include "Map/BaseMap.h"
#include "Map/WayPoint.h"

FileStream::FileStream(string path, UINT flags)
{
	this->flags = flags;

	/*
	open mode
		binary : 바이너리 모드로 파일 열기
		out : 쓰기모드
		in : 읽기 모드
	*/
	int iosFlags = ios::binary;
	iosFlags |= this->flags & FILE_STREAM_WRITE ? ios::out : 0;
	iosFlags |= this->flags & FILE_STREAM_READ ? ios::in : 0;
	
	if (this->flags & FILE_STREAM_WRITE)
	{
		out.open(path, iosFlags);
		if (out.fail())
		{
			ASSERT(false);
		}
	}
	else if(this->flags & FILE_STREAM_READ)
	{
		in.open(path, iosFlags);
		if (in.fail())
		{
			ASSERT(false);
		}
	}
}

FileStream::~FileStream()
{
	Close();
}

void FileStream::Close()
{
	if (flags & FILE_STREAM_WRITE)
	{
		out.flush(); 
		out.close();
	}
	else if (flags & FILE_STREAM_READ)
	{
		in.clear();
		in.close();
	}
}

void FileStream::Write(Rect * rect)
{
	vector<VertexColor> vertices = rect->GetVertices();
	vector<UINT> indices = rect->GetIndices();

	Write(vertices.size());
	for (VertexColor v : vertices)
		Write(v);

	Write(indices.size());
	for (UINT i : indices)
		Write(i);

	Write(rect->GetPosition());
	Write(rect->GetSize());
	Write(rect->GetRotation());
	Write(String::ToString(rect->GetShaderPath()));

}

void FileStream::Read(Rect * rect)
{
	UINT vertexCount = Read<UINT>();
	vector<VertexColor> vertices;
	for (UINT i = 0; i < vertexCount; i++)
	{
		vertices.push_back(Read<VertexColor>());
	}

	UINT indexCount = Read<UINT>();
	vector<UINT> indices;
	for (UINT i = 0; i < indexCount; i++)
	{
		indices.push_back(Read<UINT>());
	}

	D3DXVECTOR3 position = Read<D3DXVECTOR3>();
	D3DXVECTOR3 size = Read<D3DXVECTOR3>();
	float rotation = Read<float>();
	wstring shaderPath = String::ToWString(Read());

	rect->Initialize(vertices, indices, position, size, rotation, shaderPath);
}

void FileStream::Write(string value)
{
	Write(value.length());
	out.write((const char*)value.c_str(), value.length());
}

void FileStream::Read2(Rect & value)
{
	size_t size = sizeof(D3DXVECTOR3) + sizeof(D3DXVECTOR3) + sizeof(float);
	in.read((char*)&value, size);
}

Rect* FileStream::ReadRect()
{
	Rect* r;
	UINT vertexCount = Read<UINT>();
	vector<VertexColor> vertices;
	for (UINT i = 0; i < vertexCount; i++)
	{
		vertices.push_back(Read<VertexColor>());
	}

	UINT indexCount = Read<UINT>();
	vector<UINT> indices;
	for (UINT i = 0; i < indexCount; i++)
	{
		indices.push_back(Read<UINT>());
	}

	D3DXVECTOR3 position = Read<D3DXVECTOR3>();
	D3DXVECTOR3 size = Read<D3DXVECTOR3>();
	float rotation = Read<float>();
	wstring shaderPath = String::ToWString(Read());

	r->Initialize(vertices, indices, position, size, rotation, shaderPath);
	return r;
}

void FileStream::Write(BaseMap * value)
{
	Write(value->GetPosition());
	Write(value->GetSize());
	Write(String::ToString(value->GetPath()));
	Write(value->GetMonsterMaxNum());
	Write(value->GetMonsterGenCycle());
	Write(value->GetMonsterName());
	Write(value->GetGroundCount());
	/*Write(value->GetEdges()->LT.x);
	Write(value->GetEdges()->LT.y);
	Write(value->GetEdges()->RB.x);
	Write(value->GetEdges()->RB.y);*/
	for (Rect* v : value->GetGroundVec())
	{
		Write2(v);
	}
	Write(value->GetWayPointL());
	Write(value->GetWayPointR());
	

	//Write("./_Textures/TestMap2.png");
	//여기에 몬스터 객체들이랑 플레이어 객체 받아오면된다
	/*	wstring path;
	int monsterMaxNum;
	float monsterGenCycle;
	string monsterName;*/
	
}

BaseMap* FileStream::ReadBaseMap()
{
	BaseMap* baseMap;
	D3DXVECTOR3 position = Read<D3DXVECTOR3>();
	D3DXVECTOR3 size = Read<D3DXVECTOR3>();

	baseMap = new BaseMap(position, size);
	baseMap->SetSRV(String::ToWString(Read()));
	baseMap->SetMosterMaxNum(Read<int>());
	baseMap->SetMonsterGenCycle(Read<float>());
	baseMap->SetMonsterName(Read());
	//baseMap->SetDestinationPath(String::ToWString(Read()));
	baseMap->SetGroundCount(Read<int>());
	//baseMap->s
	//vector<Rect*> groundRect;
	for (int i = 0; i < baseMap->GetGroundCount(); i++)
		baseMap->SetGround(ReadRect2());
		//groundRect.push_back(ReadRect());
	
	baseMap->SetWayPointL(ReadWayPoint());
	baseMap->SetWayPointR(ReadWayPoint());
	Camera::Get()->SetTileMapBoundary(baseMap->GetEdges());

	return baseMap;
}

void FileStream::Read(WayPoint & wayPoint)
{
}

void FileStream::Write(WayPoint * wayPoint)
{
	Write(wayPoint->GetPosition());
	Write(wayPoint->GetSize());
	Write(String::ToString(wayPoint->GetDestinationPath()));
}

WayPoint* FileStream::ReadWayPoint()
{
	WayPoint* w;
	D3DXVECTOR3 position = Read<D3DXVECTOR3>();
	D3DXVECTOR3 size = Read<D3DXVECTOR3>();
	wstring destinationPath = String::ToWString(Read());

	w = new WayPoint(position, size, destinationPath);

	return w;
}

void FileStream::Write2(Rect * ract)
{
	Write(ract->GetPosition());
	Write(ract->GetSize());
	Write(ract->GetRotation());
}

Rect* FileStream::ReadRect2()
{
	Rect* t;
	D3DXVECTOR3 position =  Read<D3DXVECTOR3>();
	D3DXVECTOR3 size = Read<D3DXVECTOR3>();
	float rotation = Read<float>();

	t = new Rect(position, size, rotation);

	return t;
}



string FileStream::Read()
{
	int length = Read<UINT>();
	string temp = "";

	temp.resize(length);
	in.read((char*)&temp[0], length);
	return temp;
}


