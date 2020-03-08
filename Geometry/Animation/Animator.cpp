#include "stdafx.h"
#include "Animator.h"
#include "Utilities/Json.h"

AnimationClip::AnimationClip(wstring clipName, Texture2D * srcTex, UINT frameCount, D3DXVECTOR2 startPos, D3DXVECTOR2 endPos, bool bReverse)
	: clipName(clipName), frameCount(frameCount), bReverse(bReverse)
{
	srv = srcTex->GetSRV();

	float imageWidth = srcTex->GetWidth();
	float imageHeight = srcTex->GetHeight();

	D3DXVECTOR2 clipSize = Math::AbsVec2(endPos - startPos);
	D3DXVECTOR2 frameSize;
	frameSize.x = clipSize.x / frameCount;
	frameSize.y = clipSize.y;

	D3DXVECTOR2 texelSize = D3DXVECTOR2(1.0f / imageWidth, 1.0f / imageHeight);
	D3DXVECTOR2 texelStartPos = D3DXVECTOR2(startPos.x * texelSize.x, startPos.y * texelSize.y);
	texelFrameSize = D3DXVECTOR2(frameSize.x * texelSize.x, frameSize.y * texelSize.y);
	for (UINT i = 0; i < frameCount; i++)
	{
		D3DXVECTOR2 keyframe;
		keyframe.x = texelStartPos.x + texelFrameSize.x * i;
		keyframe.y = texelStartPos.y;
		keyframes.push_back(keyframe);
	}
}

AnimationClip::AnimationClip(const wstring & path)
{
	LoadAnimationImage(path);
	//ChangeClip(L"Stand_");
	

	// 스프라이트 하나일때 사용해야함 혹시몰라 냅둠
	/*for (auto image : imageVec)
	{
		for(auto clip : image.second.Clips)
		{
			for (auto clipData : clip.ClipDatas)
			{
				texID = srv = FindImageByName(image.first)->GetSRV();

				float imageWidth = image.second.imageSize.x;
				float imageHeight = image.second.imageSize.y;

				D3DXVECTOR2 StartPos = clipData.ClipPos;
				D3DXVECTOR2 EndPos = D3DXVECTOR2(clipData.ClipPos.x + clipData.ClipSize.x, clipData.ClipPos.y + clipData.ClipSize.y);
				D3DXVECTOR2 clipSize = Math::AbsVec2(EndPos - StartPos);

				frameCount = clip.ClipDatas.size();
				D3DXVECTOR2 frameSize = clipData.ClipSize;
				D3DXVECTOR2 texelSize = texelFrameSize = D3DXVECTOR2(frameSize.x / imageWidth, frameSize.y / imageHeight);
				D3DXVECTOR2 texelStartPos = D3DXVECTOR2(StartPos.x / imageWidth, StartPos.y / imageHeight);

				D3DXVECTOR2 keyframe;
				keyframe.x = texelStartPos.x;
				keyframe.y = texelStartPos.y;
				keyframes.push_back(keyframe);

				D3DXVECTOR2 keySize;
				keySize.x = texelSize.x;
				keySize.y = texelSize.y;

				ketframesSize.push_back(keySize);
			}
		}
	}*/

}

void AnimationClip::LoadAnimationImage(const wstring & path)
{
	ImageData imageData;

	Json::Value root;
	Json::ReadData(path, &root);

	Json::Value frames = root["frames"];
	vector<string> imageNames = frames.getMemberNames(); // 파일 안에 이미지 이름들

	Clip clipData;

	for (int i = 0; i < imageNames.size()-1; i++)
	{
		if (Path::GetFileNameWithoutIndex(imageNames[i]) != Path::GetFileNameWithoutIndex(imageNames[i + 1])
			|| (i + 2) == imageNames.size())
		{
			clipData.ClipName = String::ToWString(Path::GetFileNameWithoutIndex(imageNames[i]));
			imageData.Clips.push_back(clipData);//
		}
	}

	vector<ClipData> data;

	for (int i = 0; i < imageNames.size(); i++)
	{
		ClipData tempData;
		tempData.ClipName = String::ToWString(imageNames[i]);

		Json::Value uvData = frames[imageNames[i]]["frame"];
		Json::GetValue(uvData, "x", tempData.ClipPos.x);
		Json::GetValue(uvData, "y", tempData.ClipPos.y);
		Json::GetValue(uvData, "w", tempData.ClipSize.x);
		Json::GetValue(uvData, "h", tempData.ClipSize.y);

		data.push_back(tempData);
	}

	for (int j = 0; j < imageData.Clips.size(); j++)
		for (int i = 0; i < data.size(); i++)
			if (imageData.Clips[j].ClipName == Path::GetFileNameWithoutIndex(data[i].ClipName))
				imageData.Clips[j].ClipDatas.push_back(data[i]);

	Json::Value meta = root["meta"];
	string tempName = "";
	D3DXVECTOR2 imageSize;

	Json::GetValue(meta, "image", tempName);

	Json::Value size = meta["size"];
	int tempX, tempY;
	Json::GetValue(size, "w", tempX);
	Json::GetValue(size, "h", tempY);
	imageSize = D3DXVECTOR2((float)tempX, (float)tempY);

	string fileName = Path::GetFileNameWithoutExtension(tempName);
	imageData.imageSize = imageSize;
	imageData.jsonPath = "./_Textures/" + String::ToString(Path::GetFileName(path));

	imageVec.push_back(make_pair(fileName, imageData));

	wstring imagePath = L"./_Textures/" + String::ToWString(tempName);
	imageMap.insert(make_pair(fileName, new Texture2D(imagePath)));
}

Texture2D * AnimationClip::FindImageByName(string imageName)
{
	if (imageMap.find(imageName) != imageMap.end())
		return imageMap.find(imageName)->second;
	else
		return nullptr;
}

void AnimationClip::ChangeClip(wstring clipName)
{
	this->clipName = clipName;
	keyframes.clear();
	ketframesSize.clear();

	for (auto image : imageVec)
	{
		for (int i = 0; i < image.second.Clips.size(); i++)
		{
			if (image.second.Clips[i].ClipName == clipName)
			{
				for (int j = 0; j < image.second.Clips[i].ClipDatas.size(); j++)
				{
					ClipData clipData = image.second.Clips[i].ClipDatas[j];
					texID = srv = FindImageByName(image.first)->GetSRV();

					float imageWidth = image.second.imageSize.x;
					float imageHeight = image.second.imageSize.y;

					D3DXVECTOR2 StartPos = clipData.ClipPos;
					D3DXVECTOR2 EndPos = D3DXVECTOR2(clipData.ClipPos.x + clipData.ClipSize.x, clipData.ClipPos.y + clipData.ClipSize.y);
					clipSize = Math::AbsVec2(EndPos - StartPos);

					frameCount = image.second.Clips[i].ClipDatas.size();
					D3DXVECTOR2 frameSize = clipData.ClipSize;
					D3DXVECTOR2 texelSize = texelFrameSize = D3DXVECTOR2(frameSize.x / imageWidth, frameSize.y / imageHeight);
					D3DXVECTOR2 texelStartPos = D3DXVECTOR2(StartPos.x / imageWidth, StartPos.y / imageHeight);

					D3DXVECTOR2 keyframe;
					keyframe.x = texelStartPos.x;
					keyframe.y = texelStartPos.y;
					keyframes.push_back(keyframe);

					D3DXVECTOR2 keySize;
					keySize.x = texelSize.x;
					keySize.y = texelSize.y;

					ketframesSize.push_back(keySize);

					int a = 10;
				}
			}
		}
	}
}

Animator::Animator(AnimationClip * animation)
	: currentClip(animation)
{
	//AddAnimClip(animation);
}

Animator::~Animator()
{

}

void Animator::Update()
{
	if (bStop) return;

	if (deltaTime > playRate)
	{
		if (currentClip->GetIsReverse() == false)
		{
			currentFrameIndex++;

			if (currentFrameIndex >= currentClip->GetFrameCount() && bLoop)
			{
				currentFrameIndex = 0;
			}
			else if (currentFrameIndex >= currentClip->GetFrameCount() && !bLoop)
			{
				bStop = true;
				return;
			}

			currentFrame = currentClip->GetKeyframe(currentFrameIndex);
			currentFrameSize = currentClip->GetKeyframeSize(currentFrameIndex); // HYE
		}
		
		deltaTime = 0.0f;
	}
	else
		deltaTime += Time::Delta();
}

void Animator::AddAnimClip(AnimationClip * animClip)
{
	animClips.insert(make_pair(animClip->GetClipName(), animClip));
}

void Animator::SetCurrentAnimClip(wstring clipName)
{
	if (clipName == currentClip->GetClipName()) return;

	for (auto image : currentClip->GetImageVec())
	{
		for (int i = 0; i < image.second.Clips.size(); i++)
		{
			if (clipName == image.second.Clips[i].ClipName)
			{
				currentClip->ChangeClip(clipName);// = image.second.Clips[i].ClipDatas;
				deltaTime = 0.0f;
				
				if (currentClip->GetIsReverse())
					currentFrameIndex = currentClip->GetLastFrameIndex();
				else
					currentFrameIndex = 0;

				currentFrame = currentClip->GetKeyframe(currentFrameIndex);
				currentFrameSize = currentClip->GetKeyframeSize(currentFrameIndex); // HYE
			}
		}
	}
}
