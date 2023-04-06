#include "Sprite.h"
#include "WorldObject.h"


std::vector<Sprite*> Sprite::vpSpritesToRender;

Sprite::Sprite(WorldObject* pOwner, ImageData* pImageData, float frameTime, int layer, game::Float2 scale, game::Float2 offset)
	:
	pOwner(pOwner),
	pImageData(pImageData),
	layer(layer),
	offset(offset),
	scale(scale),
	currentAnim(0),
	currentFrame(0),
	frameTimeMax(frameTime),
	frameTimeCurrent(0),
	direction(1),
	invertedX(false),
	invertedY(false)
{
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
}

Sprite::Sprite(WorldObject* pOwner, const Sprite& sprite)
	:
	pOwner(pOwner),
	pImageData(sprite.pImageData),
	layer(sprite.layer),
	scale(sprite.scale),
	offset(sprite.offset),
	currentAnim(sprite.currentAnim),
	currentFrame(sprite.currentFrame),
	frameTimeMax(sprite.frameTimeMax),
	frameTimeCurrent(sprite.frameTimeCurrent),
	direction(sprite.direction),
	invertedX(sprite.invertedX),
	invertedY(sprite.invertedY)
{
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
}

void Sprite::Update(float deltaTime)
{
	if (pImageData->GetAnimStartFrame(currentAnim) != pImageData->GetAnimEndFrame(currentAnim))
	{
		frameTimeCurrent += deltaTime;
		if (frameTimeCurrent >= frameTimeMax && direction != 0)
		{
			frameTimeCurrent = 0;

			if (currentFrame >= pImageData->GetAnimEndFrame(currentAnim) && direction == 1)
				currentFrame = pImageData->GetAnimStartFrame(currentAnim);

			else if (currentFrame <= pImageData->GetAnimStartFrame(currentAnim) && direction == -1)
				currentFrame = pImageData->GetAnimEndFrame(currentAnim);

			else
				currentFrame += direction;
		}
	}

	vpSpritesToRender.push_back(this);
}

void Sprite::Pause() { direction = 0; }
void Sprite::PlayForwards() { direction = 1; }
void Sprite::PlayBackwards() { direction = -1; }

game::Float2 Sprite::GetLocation() { return pOwner->m_location + offset; }

game::Float2 Sprite::GetSize()
{
	float pixelSize{ 32 };
	game::Int2 dimensions{ pImageData->GetDimensionsPx() };
	return { static_cast<float>(dimensions.x) / pixelSize * scale.x, static_cast<float>(dimensions.y) / pixelSize * scale.y };
}

int Sprite::GetRenderLayer() { return layer; }

game::Rect Sprite::GetSourceRect() { return pImageData->GetCurrentRect(currentFrame); }

int Sprite::GetBitmapIndex() { return pImageData->GetTexture(); }

int Sprite::GetCurrentAnimation() { return currentAnim; }

bool Sprite::CheckInvertedX() { return invertedX; }
bool Sprite::CheckInvertedY() { return invertedY; }

void Sprite::SetAnimation(int index)
{
	currentAnim = index;
	currentFrame = pImageData->GetAnimStartFrame(index);
	frameTimeCurrent = 0;
}

void Sprite::FlipX() { invertedX = invertedX ? false : true; };
void Sprite::FlipY() { invertedY = invertedY ? false : true; };

bool Sprite::CompareLayer(Sprite* pA, Sprite* pB)
{
	return pA->GetRenderLayer() > pB->GetRenderLayer();
}

bool Sprite::CompareAbove(Sprite* pA, Sprite* pB)
{
	return pA->GetLocation().y > pB->GetLocation().y;
}

bool Sprite::CompareRowAndLeftOf(Sprite* pA, Sprite* pB)
{
	return pA->GetLocation().y == pB->GetLocation().y && pA->GetLocation().x < pB->GetLocation().x;
}
