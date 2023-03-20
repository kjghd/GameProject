#include "Sprite.h"


Sprite::Sprite(game::Float2* pOrigin, ImageData* pImageData, float frameTime, int layer, game::Float2 scale, game::Float2 offset)
	:
	origin(*pOrigin),
	pImageData(pImageData),
	layer(layer),
	offset(offset),
	scale(scale),
	currentAnim(0),
	currentFrame(0),
	frameTimeMax(frameTime),
	frameTimeCurrent(0)
{
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
}

Sprite::Sprite(game::Float2* pOrigin, const Sprite& sprite)
	:
	origin(*pOrigin),
	pImageData(sprite.pImageData),
	layer(sprite.layer),
	scale(sprite.scale),
	offset(sprite.offset),
	currentAnim(sprite.currentAnim),
	currentFrame(sprite.currentFrame),
	frameTimeMax(sprite.frameTimeMax),
	frameTimeCurrent(sprite.frameTimeCurrent)
{
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
}

void Sprite::Update(float deltaTime)
{
	if (pImageData->GetAnimStartFrame(currentAnim) != pImageData->GetAnimEndFrame(currentAnim))
	{
		frameTimeCurrent += deltaTime;
		if (frameTimeCurrent >= frameTimeMax)
		{
			frameTimeCurrent = 0;
			if (currentFrame == pImageData->GetAnimEndFrame(currentAnim))
				currentFrame = pImageData->GetAnimStartFrame(currentAnim);
			else
				++currentFrame;
		}
	}
}

game::Float2 Sprite::GetLocation()
{
	return origin + offset;
}

game::Float2 Sprite::GetSize()
{
	float pixelSize{ 32 };
	game::Int2 dimensions{ pImageData->GetDimensionsPx() };
	return { static_cast<float>(dimensions.x) / pixelSize * scale.x, static_cast<float>(dimensions.y) / pixelSize * scale.y };
}

int Sprite::GetRenderLayer()
{
	return layer;
}

game::Rect Sprite::GetSourceRect()
{
	return pImageData->GetCurrentRect(currentFrame);
}

int Sprite::GetBitmapIndex()
{
	return pImageData->GetTexture();
}

int Sprite::GetCurrentAnimation()
{
	return currentAnim;
}

void Sprite::SetAnimation(int index)
{
	currentAnim = index;
	currentFrame = pImageData->GetAnimStartFrame(index);
	frameTimeCurrent = 0;
}