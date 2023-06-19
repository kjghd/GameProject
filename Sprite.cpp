#include "Sprite.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "ScreenObject.h"
#include "ImageDataList.h"

float Sprite::pixels_per_world_unit{ 64 };
float Sprite::pixels_per_screen_unit{ 32 };

std::vector<Sprite*> Sprite::vpSpritesToRender;

Sprite::Sprite(GameObject* pOwner, ImageData* pImageData, float frameTime, int layer, game::float2 scale, game::float2 offset)
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
	invertedY(false),
	visible(true),
	active(true)
{
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
	m_tag = "SPRT";
}
Sprite::Sprite(GameObject* pOwner, const Sprite& sprite)
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
	invertedY(sprite.invertedY),
	visible(sprite.visible),
	active(sprite.active)
{
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
	m_tag = "SPRT";
}
Sprite::Sprite(GameObject* pOwner, std::istream& is)
	:
	pOwner(pOwner),
	offset{ std::stof(FileWritable::GetNextValue(is)), std::stof(FileWritable::GetNextValue(is)) },
	scale{ std::stof(FileWritable::GetNextValue(is)), std::stof(FileWritable::GetNextValue(is)) },
	layer(std::stoi(FileWritable::GetNextValue(is))),
	pImageData(ImageDataList::Get(FileWritable::GetNextValue(is))),
	currentFrame(std::stoi(FileWritable::GetNextValue(is))),
	currentAnim(std::stoi(FileWritable::GetNextValue(is))),
	frameTimeMax(std::stof(FileWritable::GetNextValue(is))),
	frameTimeCurrent(std::stof(FileWritable::GetNextValue(is))),
	direction(std::stoi(FileWritable::GetNextValue(is))),
	invertedX(FileWritable::GetNextValue(is) == "1" ? true : false),
	invertedY(FileWritable::GetNextValue(is) == "1" ? true : false),
	visible(FileWritable::GetNextValue(is) == "1" ? true : false),
	active(FileWritable::GetNextValue(is) == "1" ? true : false)

{
	m_tag = "SPRT";
}


void Sprite::Update(float deltaTime)
{
	// Animation
	if (active)
	{
		if (pImageData->GetAnimStartFrame(currentAnim) != pImageData->GetAnimEndFrame(currentAnim))
		{
			frameTimeCurrent += deltaTime;
			if (frameTimeCurrent >= frameTimeMax && direction != 0)
			{
				frameTimeCurrent = 0;

				// Set to start
				if (currentFrame >= pImageData->GetAnimEndFrame(currentAnim) && direction == 1)
					currentFrame = pImageData->GetAnimStartFrame(currentAnim);

				// Set to end
				else if (currentFrame <= pImageData->GetAnimStartFrame(currentAnim) && direction == -1)
					currentFrame = pImageData->GetAnimEndFrame(currentAnim);

				// Continue
				else
					currentFrame += direction;
			}
		}
	}

	// Render queue
	if (visible) vpSpritesToRender.push_back(this);
}

void Sprite::Pause() { active = false;  direction = 0; }
void Sprite::PlayForwards() { active = true; direction = 1; }
void Sprite::PlayBackwards() { active = true;  direction = -1; }


void Sprite::SetFrame(int index)
{
	currentAnim = 0;
	currentFrame = index;
	frameTimeCurrent = 0;
	Pause();
}

void Sprite::SetAnimation(std::string name)
{
	currentAnim = pImageData->GetAnimIndex(name);
	currentFrame = pImageData->GetAnimStartFrame(currentAnim);
	frameTimeCurrent = 0;
}


void Sprite::FlipX() { invertedX = invertedX ? false : true; };
void Sprite::FlipY() { invertedY = invertedY ? false : true; };

GameObject* Sprite::GetOwner() { return pOwner; }

game::float2 Sprite::GetLocation() { return pOwner->m_location + offset; }

game::float2 Sprite::GetSize()
{
	float px_per_unit{ 1.f };

	if (dynamic_cast<WorldObject*>(pOwner)) px_per_unit = pixels_per_world_unit;
	else if (dynamic_cast<ScreenObject*>(pOwner)) px_per_unit = pixels_per_screen_unit;

	game::int2 dimensions{ pImageData->GetDimensionsPx() };
	return {
		static_cast<float>(dimensions.x) / px_per_unit * scale.x,
		static_cast<float>(dimensions.y) / px_per_unit * scale.y
	};
}

int Sprite::GetRenderLayer() { return layer; }
game::rect Sprite::GetSourceRect() { return pImageData->GetCurrentRect(currentFrame); }
int Sprite::GetBitmapIndex() { return pImageData->GetTexture(); }
int Sprite::GetCurrentAnimation() { return currentAnim; }
std::string Sprite::GetAnimName()
{
	return pImageData->GetAnimName(currentAnim);
}

bool Sprite::CheckInvertedX() { return invertedX; }
bool Sprite::CheckInvertedY() { return invertedY; }


bool Sprite::CompareLayer(Sprite* pA, Sprite* pB)
{
	return pA->GetRenderLayer() > pB->GetRenderLayer();
}

bool Sprite::CompareAbove(Sprite* pA, Sprite* pB)
{
	return pA->pOwner->m_location.y > pB->pOwner->m_location.y;
}

bool Sprite::CompareRowAndLeftOf(Sprite* pA, Sprite* pB)
{
	//game::Float2 sizeA{ pA->GetSize() };
	//game::Float2 locA{ pA->GetLocation() };
	//
	//game::Float2 sizeB{ pB->GetSize() };
	//game::Float2 locB{ pB->GetLocation() };
	//
	//return (locA.y - sizeA.y / 2.f) == (locB.y - sizeB.y / 2.f) &&
	//	(locA.x - sizeA.x / 2.f) < (locB.x - sizeB.x / 2.f);

	//return pA->GetLocation().y == pB->GetLocation().y && pA->GetLocation().x < pB->GetLocation().x;

	return pA->pOwner->m_location.y == pB->pOwner->m_location.y && pA->pOwner->m_location.x < pB->pOwner->m_location.x;
}

bool Sprite::Obstructing(Sprite* pA, Sprite* pB)
{
	game::float2 sizeA{ pA->GetSize() };
	game::float2 locA{ pA->GetLocation() };
	
	game::float2 sizeB{ pB->GetSize() };
	game::float2 locB{ pB->GetLocation() };

	if (locA.x - sizeA.x / 2.f < locB.x + sizeB.x / 2.f && // al < br
		locA.x + sizeA.x / 2.f > locB.x - sizeB.x / 2.f && // ar > bl
		locA.y + sizeA.y / 2.f > locB.y - sizeB.y / 2.f && // at > bb
		locA.y - sizeA.y / 2.f < locB.y + sizeB.y / 2.f && // ab < bt
		CompareAbove(pB, pA) &&
		//CompareRowAndLeftOf(pB, pA) &&
		pA->layer == pB->layer)
	{
		return true;
	}
	else return false;
	
	//return locA.y - sizeA.y / 2.f > locB.y - sizeB.y / 2.f && ;
}

void Sprite::WriteData(std::ostream& os)
{
	os << offset.x					<< ',';
	os << offset.y					<< ',';
	os << scale.x					<< ',';
	os << scale.y					<< ',';
	os << layer						<< ',';
	os << pImageData->GetFilename().c_str() << ',';
	os << currentFrame				<< ',';
	os << currentAnim				<< ',';
	os << frameTimeMax				<< ',';
	os << frameTimeCurrent			<< ',';
	os << direction					<< ',';
	os << invertedX					<< ',';
	os << invertedY					<< ',';
	os << visible					<< ',';
	os << active;
}
