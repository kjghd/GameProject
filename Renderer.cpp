#include "Renderer.h"
#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"

#include "Sprite.h"

#include "Camera.h"


#include <algorithm>

Renderer::Renderer()
	:
	m_pGraphics(nullptr),
	m_pSceneController(nullptr),
	m_debug(false)
{
}

void Renderer::Init(Graphics* pGraphics, SceneController* pSceneController)
{
	m_pGraphics = pGraphics;
	m_pSceneController = pSceneController;
}

void Renderer::Render()
{
	m_pGraphics->BeginDraw();
	m_pGraphics->ClearScreen();



	Camera* pCamera{ m_pSceneController->GetActive()->GetCamera() };


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
				game::rect rect{
					pCamera->WorldTransformToScreenRect(
						pSprite->GetLocation(),
						pSprite->GetSize()
					)
				};

				game::rect region{ pSprite->GetSourceRect() };

				float opacity{ 1.f };

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
		
		game::float2 location{ pSprite->GetLocation() + Camera::m_screenResolution / ScreenObject::px_per_su / 2.f};
		game::float2 size{ pSprite->GetSize() };
		game::rect rect{
			location.x * ScreenObject::px_per_su - size.x / 2.f * ScreenObject::px_per_su,
			location.y * ScreenObject::px_per_su + size.y / 2.f * ScreenObject::px_per_su,
			location.x * ScreenObject::px_per_su + size.x / 2.f * ScreenObject::px_per_su,
			location.y * ScreenObject::px_per_su - size.y / 2.f * ScreenObject::px_per_su
		};
		game::rect sourceRect{ pSprite->GetSourceRect() };

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