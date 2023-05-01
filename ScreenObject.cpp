#include "ScreenObject.h"


game::float2 ScreenObject::screenRes = { 1280,720 };
float ScreenObject::px_per_su = 64.f;

ScreenObject::ScreenObject(ImageData* pImageData, int layer, game::float2 scale, game::float2 offset, float frameTime)
	:
	GameObject(pImageData, layer, scale, offset, frameTime)
{
}
ScreenObject::ScreenObject(const ScreenObject& screenObject)
	:
	GameObject(screenObject)
{
}
ScreenObject::ScreenObject(const Sprite& sprite)
	:
	GameObject(sprite)
{
}

void ScreenObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
}

void ScreenObject::SetLocaion_px(game::float2 pixels)
{
	m_location.x = (pixels.x - screenRes.x / 2) / px_per_su;
	m_location.y = (pixels.y - screenRes.y / 2) / px_per_su;
}
void ScreenObject::SetLocaion_su(game::float2 screenUnits)
{
	m_location = screenUnits;
}
void ScreenObject::SetLocation_percentage(game::float2 percentage)
{
	// Broken
	m_location.x = screenRes.x * percentage.x / px_per_su ;
	m_location.y = screenRes.y * percentage.y / px_per_su ;
}

int ScreenObject::GetBitmapIndex()
{
	return m_sprite.GetBitmapIndex();
}
int ScreenObject::GetRenderLayer()
{
	return m_sprite.GetRenderLayer();
}
game::rect ScreenObject::GetSourceRect()
{
	return m_sprite.GetSourceRect();
}
game::rect ScreenObject::GetScreenRect()
{
	game::float2 size{ m_sprite.GetSize() };

	return{
		(m_location.x - size.x / 2) * px_per_su,
		(m_location.y - size.y / 2) * px_per_su,
		(m_location.x + size.x / 2) * px_per_su,
		(m_location.y + size.y / 2) * px_per_su
	};
}

bool ScreenObject::InvertedX()
{
	return m_sprite.CheckInvertedX();
}
bool ScreenObject::InvertedY()
{
	return m_sprite.CheckInvertedY();
}

bool ScreenObject::CompareRenderOrder_Under(ScreenObject* screenObjectA, ScreenObject* screenObjectB)
{
	return screenObjectA->GetRenderLayer() > screenObjectB->GetRenderLayer();
}

std::string ScreenObject::Serialise()
{
	std::string str;

	//str += GameObject::Serialise();
	//str += game::DataToString<game::float2>(screenRes);
	//str += game::DataToString<float>(px_per_su);

	return str;
}

/* Preview */


SO_Preview::SO_Preview(game::float2 size, ImageData* pImageData, int layer, game::float2 scale, game::float2 offset, float frameTime)
	:
	ScreenObject(pImageData, layer, scale, offset, frameTime),
	pPreviewed(nullptr),
	previewSize(size)
{
}
SO_Preview::SO_Preview(const SO_Preview& box)
	:
	ScreenObject(box),
	pPreviewed(new Sprite(*box.pPreviewed)),
	previewSize(box.previewSize)
{
}
SO_Preview::SO_Preview(const Sprite& sprite, game::float2 size)
	:
	ScreenObject(sprite),
	pPreviewed(nullptr),
	previewSize(size)
{
}
SO_Preview::~SO_Preview()
{
	delete pPreviewed;
	pPreviewed = nullptr;
}

void SO_Preview::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
	pPreviewed->Update(deltaTime);
}

void SO_Preview::SetPreviewed(Sprite* pSprite)
{
	game::float2 newSize{};
	if (pSprite->GetSize().x / pSprite->GetSize().y < 1.f)
	{
		float xRatio{ pSprite->GetSize().x / pSprite->GetSize().y };
		newSize.x = xRatio * previewSize.x;
		newSize.y = previewSize.y;
	}
	else if (pSprite->GetSize().y / pSprite->GetSize().x < 1.f)
	{
		float yRatio{ pSprite->GetSize().y / pSprite->GetSize().x };
		newSize.x = previewSize.x;
		newSize.y = yRatio * previewSize.y;
	}
	else
	{
		newSize = pSprite->GetSize();
	}


	//Sprite* s{ new Sprite(this, ) };

	pPreviewed = pSprite;
}
