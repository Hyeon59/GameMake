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
	void SaveGameMap(const wstring & path = L"");  //林盔
	void LoadGameMap(const wstring & path = L"");  //林盔

	GameManager();
	~GameManager();

	int a = 0;
	int b = 0;
	class Player* player = nullptr;
	class Monster* monster = nullptr;
	//class Rect* land = nullptr;

	class Character* testCharacter;//林盔

	D3DXVECTOR3 worldPos = Value::Zero;//林盔

	
	class BaseMap* tempMap = nullptr;//林盔
	class BaseMap* testMap = nullptr;//林盔
	
	class WayPoint* w1 = nullptr;//林盔

};