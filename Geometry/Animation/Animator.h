#pragma once

struct ClipData
{
	wstring ClipName;
	D3DXVECTOR2 ClipPos;
	D3DXVECTOR2 ClipSize;
};

struct Clip
{
	wstring ClipName;
	vector<ClipData> ClipDatas;
};

struct ImageData
{
	string jsonPath;
	vector<Clip> Clips;
	D3DXVECTOR2 imageSize;
};

class AnimationClip
{
public:
	AnimationClip(
		wstring clipName,
		Texture2D* srcTex,
		UINT frameCount,
		D3DXVECTOR2 startPos,
		D3DXVECTOR2 endPos,
		bool bReverse = false
	);
	AnimationClip(const wstring& path = L""); // HYE
	~AnimationClip() = default;

	void LoadAnimationImage(const wstring& path = L""); // JSON 불러오기
	void ChangeClip(wstring clipName); // Clip 바꾸기
	Texture2D* FindImageByName(string imageName); 

	wstring GetClipName() { return clipName; }
	D3DXVECTOR2 GetKeyframe(UINT index) { return keyframes[index]; } // 프레임 StartUV
	D3DXVECTOR2 GetKeyframeSize(UINT index) { return ketframesSize[index]; } // 프레임 UV사이즈
	D3DXVECTOR2 GetTexelFrameSize() { return texelFrameSize; } // JSON 아닐때 필요한 프레임 UV 사이즈
	D3DXVECTOR2 GetClipSize() { return clipSize; } // 프레임 실제 사이즈

	UINT GetFrameCount() { return frameCount; }
	UINT GetLastFrameIndex() { return keyframes.size() - 1; }

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	bool GetIsReverse() { return bReverse; }

	auto GetImageVec()& { return imageVec; }

private:
	wstring clipName = L"";
	vector<D3DXVECTOR2> keyframes; // 프레임 StartUV Vector
	vector<D3DXVECTOR2> ketframesSize; // 프레임 UV사이즈 Vector
	
	D3DXVECTOR2 texelFrameSize = D3DXVECTOR2(0, 0); // JSON 아닐때 필요한 프레임 UV 사이즈
	D3DXVECTOR2 clipSize = D3DXVECTOR2(0, 0); // 프레임 실제 사이즈

	UINT frameCount = 0; //총 프레임 개수

	ImTextureID texID = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	vector<pair<string, ImageData>> imageVec;
	unordered_map<string, Texture2D*> imageMap;

	bool bReverse = false;

	vector<AnimationClip*> animationClip;
};

class Animator
{
public:
	Animator(AnimationClip* animation);
	~Animator();

	void Update();
	void AddAnimClip(AnimationClip* animClip);

	D3DXVECTOR2 GetCurrentFrame() { return currentFrame; } // 현재 프레임 시작 UV
	D3DXVECTOR2 GetCurrentFrameSize() { return currentFrameSize; } // 현재 프레임 UV 사이즈
	D3DXVECTOR2 GetTexelFrameSize() { return currentClip->GetTexelFrameSize(); } // JSON 아닐때 필요한 프레임 UV 사이즈
	D3DXVECTOR2 GetClipSize() { return currentClip->GetClipSize(); } // 프레임 실제 사이즈

	ID3D11ShaderResourceView* GetCurrentSRV() { return currentClip->GetSRV(); }

	void SetCurrentAnimClip(wstring clipName);
	void SetPlayRate(float playRate) { this->playRate = playRate; }
	void SetLoop(bool bLoop) { this->bLoop = bLoop; }
	void SetStop(bool bStop) { this->bStop = bStop; }
	bool GetLoop() { return bLoop; }
	bool GetStop() { return bStop; }

private:
	unordered_map<wstring, AnimationClip*> animClips;
	AnimationClip* currentClip = nullptr;

	float deltaTime = 0.0f;
	UINT currentFrameIndex = 0;

	D3DXVECTOR2 currentFrame = D3DXVECTOR2(0, 0); // 현재 프레임 시작 UV
	D3DXVECTOR2 currentFrameSize = D3DXVECTOR2(0, 0); // 현재 프레임 UV 사이즈

	float playRate = 1.0f / 15.0f;
	bool bLoop = true;
	bool bStop = false;
};