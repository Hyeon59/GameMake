#pragma once

class GameManager : public Singleton<GameManager>
{
	friend class Singleton<GameManager>;

public:
	void Update();
	void Render();
	void GUI();

	void HYEON();

private:
	void SaveGameMap(const wstring & path = L"");  //�ֿ�
	void LoadGameMap(const wstring & path = L"");  //�ֿ�

	GameManager();
	~GameManager();

	int a = 0;
	int b = 0;
	class Player* player = nullptr;
	class Monster* monster = nullptr;
	//class Rect* land = nullptr;

	class Character* testCharacter;//�ֿ�

	D3DXVECTOR3 worldPos = Value::Zero;//�ֿ�

	
	class BaseMap* tempMap = nullptr;//�ֿ�
	class BaseMap* testMap = nullptr;//�ֿ�
	
	class WayPoint* w1 = nullptr;//�ֿ�

};