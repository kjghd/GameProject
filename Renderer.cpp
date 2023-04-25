#include "Renderer.h"
#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"

#include "Sprite.h"

#include <algorithm>

Renderer::Renderer()
	:
	m_pGraphics(nullptr),
	m_pScene(nullptr),
	m_pCamera(nullptr),
	m_debug(false)
{
}

void Renderer::Init(Graphics* pGraphics, Scene* pScene)
{
	m_pScene = pScene;
	m_pGraphics = pGraphics;
}

void Renderer::Render()
{
	m_pGraphics->BeginDraw();
	m_pGraphics->ClearScreen();

	m_pCamera = &m_pScene->GetCamera();

	/* sorting */

	std::vector<Sprite*> vpUnsorted{ Sprite::vpSpritesToRender };

	// Screen sorting.
	std::vector<Sprite*> vpSOSorted;
	
	bool worldOnly{ false };
	while (!worldOnly)
	{
		int position{ 0 };
		bool found{ false };
		for (auto& pSprite : vpUnsorted)
		{
			if (dynamic_cast<ScreenObject*>(pSprite->GetOwner()))
			{
				found = true;
				vpSOSorted.push_back(pSprite);
				vpUnsorted.erase(vpUnsorted.begin() + position);
				break;
			}
			++position;
		}

		if (!found) worldOnly = true;
	}

	std::sort(vpSOSorted.begin(), vpSOSorted.end(), Sprite::CompareLayer);

	// World sorting.
	std::vector<Sprite*> vpWOSorted;

	while (!vpUnsorted.empty())
	{
		// Find lowest sprite.
		Sprite* pLowestFound{ nullptr };
		for (auto& pSprite : vpUnsorted)
		{
			if (!pLowestFound)
				pLowestFound = pSprite;

			else if (pSprite != pLowestFound)
			{
				if (Sprite::CompareAbove(pSprite, pLowestFound))
					pLowestFound = pSprite;

				else if (Sprite::CompareRowAndLeftOf(pSprite, pLowestFound))
					pLowestFound = pSprite;
			}
		}

		// Update Lists.
		if (pLowestFound)
		{
			// Remove from unsorted.
			int i{ 0 };
			for (auto& pSprite : vpUnsorted)
			{
				if (pSprite == pLowestFound)
				{
					vpUnsorted.erase(vpUnsorted.begin() + i);
					break;
				}

				++i;
			}

			// Add to sorted.
			vpWOSorted.push_back(pLowestFound);
		}
	}


	/* World Objects */

	for (size_t i = SL_COUNT; i > 0 ; --i)
		for (auto& pSprite : vpWOSorted)
			if (pSprite->GetRenderLayer() == i)
			{
				game::Rect rect{
					m_pCamera->WorldTransformToScreenRect(
					pSprite->GetLocation(),
					pSprite->GetSize()
					)
				};

				game::Rect region{ pSprite->GetSourceRect() };

				float opacity{ 1.f };
				//if (pSprite != &m_pScene->GetPlayer().m_sprite && Sprite::Obstructing(pSprite, &m_pScene->GetPlayer().m_sprite))
				//	opacity = .2f;

				m_pGraphics->DrawBitmapRegion(
					D2D1::RectF(rect.l, rect.t, rect.r, rect.b),
					pSprite->GetBitmapIndex(),
					D2D1::RectF(region.l, region.t, region.r, region.b),
					opacity,
					pSprite->CheckInvertedX(),
					pSprite->CheckInvertedY()
				);
			}


	/* User Interface */

	for (auto& pSprite : vpSOSorted)
	{
		
		game::Float2 location{ pSprite->GetLocation() };
		game::Float2 size{ pSprite->GetSize() };
		game::Rect rect{
			location.x * ScreenObject::px_per_su - size.x / 2.f * ScreenObject::px_per_su,
			location.y * ScreenObject::px_per_su + size.y / 2.f * ScreenObject::px_per_su,
			location.x * ScreenObject::px_per_su + size.x / 2.f * ScreenObject::px_per_su,
			location.y * ScreenObject::px_per_su - size.y / 2.f * ScreenObject::px_per_su
		};
		game::Rect sourceRect{ pSprite->GetSourceRect() };

		m_pGraphics->DrawBitmapRegion(
			D2D1::RectF(rect.l, rect.t, rect.r, rect.b),
			pSprite->GetBitmapIndex(),
			D2D1::RectF(sourceRect.l, sourceRect.t, sourceRect.r, sourceRect.b),
			1,
			pSprite->CheckInvertedX(),
			pSprite->CheckInvertedY()
		);
	}


	Sprite::vpSpritesToRender.clear();

	m_pGraphics->EndDraw();

}