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
	

	/* World Objects */

	// Sort for rendering.
	std::vector<Sprite*> vpWOUnsorted{ Sprite::vpSpritesToRender };
	std::vector<Sprite*> vpWOSorted;
	while (!vpWOUnsorted.empty())
	{
		// Find lowest sprite.
		Sprite* pLowestFound{ nullptr };
		for (auto& pSprite : vpWOUnsorted)
		{
			if (!pLowestFound)
				pLowestFound = pSprite;

			else if (pSprite != pLowestFound)
			{
				if (Sprite::CompareLayer(pSprite, pLowestFound))
					pLowestFound = pSprite;

				else if (Sprite::CompareAbove(pSprite, pLowestFound))
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
			for (auto& pSprite : vpWOUnsorted)
			{
				if (pSprite == pLowestFound)
				{
					vpWOUnsorted.erase(vpWOUnsorted.begin() + i);
					break;
				}

				++i;
			}

			// Add to sorted.
			vpWOSorted.push_back(pLowestFound);
		}
	}

	// Render
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

				m_pGraphics->DrawBitmapRegion(
					D2D1::RectF(rect.l, rect.t, rect.r, rect.b),
					pSprite->GetBitmapIndex(),
					D2D1::RectF(region.l, region.t, region.r, region.b),
					1,
					pSprite->CheckInvertedX(),
					pSprite->CheckInvertedY()
				);
			}

	Sprite::vpSpritesToRender.clear();


	/* User Interface */

	//if (m_pScene->state == SState_Pause)
	//{
	//	// Sort for rendering.
	//	std::vector<ScreenObject*> vpSOUnsorted{ m_pScene->vpScreenObjects };
	//	std::vector<ScreenObject*> vpSOSorted;
	//	while (!vpSOUnsorted.empty())
	//	{
	//		// Find lowest sprite.
	//		ScreenObject* pLowestFound{ nullptr };
	//		for (auto& pScreenObject : vpSOUnsorted)
	//		{
	//			if (!pLowestFound)
	//				pLowestFound = pScreenObject;
	//
	//			else if (pScreenObject != pLowestFound)
	//			{
	//				if (ScreenObject::CompareRenderOrder_Under(pScreenObject, pLowestFound))
	//					pLowestFound = pScreenObject;
	//			}
	//		}
	//
	//		// Update Lists.
	//		if (pLowestFound)
	//		{
	//			// Remove from unsorted.
	//			int i{ 0 };
	//			for (auto& pScreenObject : vpSOUnsorted)
	//			{
	//				if (pScreenObject == pLowestFound)
	//				{
	//					vpSOUnsorted.erase(vpSOUnsorted.begin() + i);
	//					break;
	//				}
	//
	//				++i;
	//			}
	//
	//			// Add to sorted.
	//			vpSOSorted.push_back(pLowestFound);
	//		}
	//	}
	//
	//	// Render
	//	for (size_t i = SL_COUNT; i > 0; --i)
	//		for (auto& pScreenObject : vpSOSorted)
	//			if (pScreenObject->GetRenderLayer() == i)
	//			{
	//				game::Rect screen{ pScreenObject->GetScreenRect() };
	//
	//				game::Rect region{ pScreenObject->GetSourceRect() };
	//
	//				m_pGraphics->DrawBitmapRegion(
	//					D2D1::RectF(screen.l, screen.t, screen.r, screen.b),
	//					pScreenObject->GetBitmapIndex(),
	//					D2D1::RectF(region.l, region.t, region.r, region.b),
	//					1,
	//					pScreenObject->InvertedX(),
	//					pScreenObject->InvertedY()
	//				);
	//			}
	//}
	//else if (m_pScene->state == SState_Run)
	//{
	//	// Block preview
	//	game::Rect region{ m_pScene->prefabs.GetWorldObject(m_pScene->current_prefab)->m_sprite.GetSourceRect() };
	//
	//	m_pGraphics->DrawBitmapRegion(
	//		D2D1::RectF(0, 0, 60, 60),
	//		m_pScene->prefabs.GetWorldObject(m_pScene->current_prefab)->m_sprite.GetBitmapIndex(),
	//		D2D1::RectF(region.l, region.t, region.r, region.b)
	//);
	
	// Cursor
	game::Float2 loc{
	m_pCamera->ScreenLocToWorldLoc(
			m_pScene->GetInput().GetMouseLoc().x,
			m_pScene->GetInput().GetMouseLoc().y
		)
	};
	game::Float2 locRounded{ roundf(loc.x), roundf(loc.y) };
	
	game::Float2 locScreen{
	m_pCamera->WorldLocToScreenLoc(
			locRounded.x,
			locRounded.y
		)
	};
	
	m_pGraphics->DebugCircle({ locScreen.x, locScreen.y }, m_pCamera->WU_to_SU(.5f));

	//}

	m_pGraphics->EndDraw();

}