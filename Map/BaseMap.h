#pragma once
class Rect;
class WayPoint;
class BaseMap
{
public:

	BaseMap(D3DXVECTOR3 position, D3DXVECTOR3 size, float rotation = 0);
	~BaseMap();

	void TransformVertices();

	void Update();
	void Render();
	bool CheckEnd();
	void Reposition(D3DXVECTOR3 newPos);

	//TextureEdges* GetEdges() { return &edge; }
	RectEdges* GetEdges() { return &edge; }
	D3DXVECTOR3 GetPosition() { return position; }
	D3DXVECTOR3 GetSize() { return size; }

	//void Move(D3DXVECTOR3 position);
	//bool Intersect(TextureRect* other);
	void CheckIntersect(bool bIntersect) { this->bIntersect = bIntersect; }

	void SetSRV(wstring path);
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

	D3DXVECTOR3 GetMousePos();

	//맵정보 쓰기읽기용  몬스터랑 플레이어는 나중에추가
	wstring GetPath() { return path; }
	int GetMonsterMaxNum() { return monsterMaxNum; }
	float GetMonsterGenCycle() { return monsterGenCycle; }
	string GetMonsterName() { return monsterName; }
	wstring GetDestinationPath() { return destinationPath; }
	int GetGroundCount() { return groundCount; }
	vector<Rect*> GetGroundVec() { return grounds; }
	WayPoint* GetWayPointL() { return lWayPoint; }
	WayPoint* GetWayPointR() { return rWayPoint; }

	void SetMosterMaxNum(int monsterMaxNum) { this->monsterMaxNum = monsterMaxNum; }
	void SetMonsterGenCycle(float monsterGenCycle) { this->monsterGenCycle = monsterGenCycle; }
	void SetMonsterName(string monsterName) { this->monsterName = monsterName; }
	void SetCurrentMapPath(wstring path) { this->path = path; }
	void SetDestinationPath(wstring) { this->destinationPath = destinationPath; }
	void SetGroundCount(int groundCount) { this->groundCount = groundCount; }
	void SetGround(Rect* ground);
	void SetWayPointL(WayPoint* lwayPoint) { this->lWayPoint = lwayPoint; }
	void SetWayPointR(WayPoint* rwayPoint) { this->rWayPoint = rwayPoint; }
	void SetRectEdges(RectEdges* rectEdges) { this->edge = *rectEdges; }

	void MakeGroundTool(D3DXVECTOR3 mousePos,D3DXVECTOR3 groundSize,float groundRotation);

	void GUI();
private:
	vector<VertexTexture> vertices;
	VertexBuffer* VB = nullptr;

	vector<UINT> indices;
	IndexBuffer* IB = nullptr;

	InputLayout* IL = nullptr;

	VertexShader* VS = nullptr;
	PixelShader* PS = nullptr;

	D3DXMATRIX world;

	D3DXMATRIX S, R, T;

	WorldBuffer* WB = nullptr;

	D3DXVECTOR3 position = Value::Zero;
	D3DXVECTOR3 size = Value::One;
	float rotation = 0.0f;

	bool bIntersect = false;
	//TextureEdges edge;
	RectEdges edge;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11ShaderResourceView* nullView = nullptr;

	ID3D11BlendState* bsState_disabled;
	ID3D11BlendState* bsState_enabled;

	D3DXVECTOR3 worldPos = Value::Zero;

	wstring path;
	int monsterMaxNum;
	float monsterGenCycle;
	string monsterName;
	wstring destinationPath;
	//맵은 구조물들의 정보또한 가지고 있어야한다
	//그러므로 렉트 구조체들의 위치또한 가지고있어야함

	int groundCount=0;
	D3DXVECTOR3 groundSize = Value::Zero;
	float groundRotation = 0.0f;
	vector<Rect*> grounds; // 바닥

	WayPoint* lWayPoint = nullptr;
	WayPoint* rWayPoint = nullptr;

};