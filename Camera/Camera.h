#pragma once

class Camera : public Singleton<Camera>
{
public:
	friend Singleton;
	/*static void Create();
	static void Delete();
	static Camera* Get();*/

	void Update();
	void Render();
	void SetPosition(D3DXVECTOR3 position, bool bOffCenter = true);

	void UnProject(D3DXVECTOR3* out, D3DXVECTOR3 source, D3DXMATRIX world);
	D3DXVECTOR3 GetPosition() { return position; }//аж©Ь
	void SetTileMapBoundary(RectEdges* edges) { this->mapBoundary = *edges; }
private:
	void ZoomIn();
	void ZoomOut();

	Camera();
	~Camera();

	static Camera* instance;

	void UpdateView();
	void UpdateProj();

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	D3DXMATRIX view;
	D3DXMATRIX proj;

	VPBuffer* VP;

	RectEdges mapBoundary;

	float tempX = 0;
	float tempY = 0;

	D3DXVECTOR4 screenSize = { 0, (float)WinMaxHeight, (float)WinMaxWidth, 0 };
	float ratio = (float)WinMaxWidth / (float)WinMaxHeight;
	float zoomSpeed = 20.0f;
};