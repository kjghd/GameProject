#include "Renderer.h"
#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"

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
	

	// Sort for rendering.
	std::vector<GameObject*> vpUnsorted{ m_pScene->vpGameObjects };
	std::vector<GameObject*> vpSorted;
	while (!vpUnsorted.empty())
	{
		// Find lowest sprite.
		GameObject* pLowestFound{ nullptr };
		for (auto& pGameObject : vpUnsorted)
		{
			if (!pLowestFound)
				pLowestFound = pGameObject;

			else if (pGameObject != pLowestFound)
			{
				if (GameObject::CompareRenderOrder_Under(pGameObject, pLowestFound))
					pLowestFound = pGameObject;

				else if (GameObject::CompareRenderOrder_Above(pGameObject, pLowestFound))
					pLowestFound = pGameObject;

				else if (GameObject::CompareRenderOrder_RowAndLeftOf(pGameObject, pLowestFound))
					pLowestFound = pGameObject;
			}
		}

		// Update Lists.
		if (pLowestFound)
		{
			// Remove from unsorted.
			int i{ 0 };
			for (auto& pGameObject : vpUnsorted)
			{
				if (pGameObject == pLowestFound)
				{
					vpUnsorted.erase(vpUnsorted.begin() + i);
					break;
				}

				++i;
			}

			// Add to sorted.
			vpSorted.push_back(pLowestFound);
		}
	}

	// Render
	for (size_t i = SL_COUNT; i > 0 ; --i)
		for (auto& pGameObject : vpSorted)
			if (pGameObject->m_sprite.GetRenderLayer() == i)
			{
				game::Rect rect{
					m_pCamera->WorldTransformToScreenRect(
					pGameObject->m_sprite.GetLocation(),
					pGameObject->m_sprite.GetSize()
					)
				};

				game::Rect region{ pGameObject->m_sprite.GetSourceRect() };

				m_pGraphics->DrawBitmapRegion(
					D2D1::RectF(rect.l, rect.t, rect.r, rect.b),
					pGameObject->m_sprite.GetBitmapIndex(),
					D2D1::RectF(region.l, region.t, region.r, region.b)
				);
			}

	// Collision Debug
	if (m_debug)
	{
		for (auto& pGameObject : vpSorted)
		{
			if (dynamic_cast<Box*>(pGameObject))
			{
				Box* pTile{ dynamic_cast<Box*>(pGameObject) };

				game::Float2 topRight{
					m_pCamera->WorldLocToScreenLoc(
						pTile->m_location.x + pTile->m_collider.size.x / 2,
						pTile->m_location.y + pTile->m_collider.size.y / 2
					)
				};
				game::Float2 bottomLeft{
					m_pCamera->WorldLocToScreenLoc(
						pTile->m_location.x - pTile->m_collider.size.x / 2,
						pTile->m_location.y - pTile->m_collider.size.y / 2
					)
				};

				m_pGraphics->DebugBox(D2D1::RectF(bottomLeft.x, topRight.y, topRight.x, bottomLeft.y));
			}
			else if (dynamic_cast<Ball*>(pGameObject))
			{
				Ball* pBall{ dynamic_cast<Ball*>(pGameObject) };

				game::Float2 loc_collider{ m_pCamera->WorldLocToScreenLoc(pBall->m_collider.origin.x, pBall->m_collider.origin.y) };
				float radius_collider{ m_pCamera->WU_to_SU(pBall->m_collider.radius) };

				m_pGraphics->DebugCircle(D2D1::Point2F(loc_collider.x, loc_collider.y), radius_collider);

				if (dynamic_cast<Character*>(pGameObject))
				{
					Character* pCharacter{ dynamic_cast<Character*>(pGameObject) };

					game::Float2 loc_view{ m_pCamera->WorldLocToScreenLoc(pCharacter->m_viewRange.origin.x, pCharacter->m_viewRange.origin.y) };
					float radius_view{ m_pCamera->WU_to_SU(pCharacter->m_viewRange.radius) };

					m_pGraphics->DebugCircle(D2D1::Point2F(loc_view.x, loc_view.y), radius_view);
				}
			}
		}

	}


	// Block preview
	game::Rect region{ m_pScene->prefabs.Get(m_pScene->current_prefab)->m_sprite.GetSourceRect() };

	m_pGraphics->DrawBitmapRegion(
		D2D1::RectF(0, 0, 60, 60),
		m_pScene->prefabs.Get(m_pScene->current_prefab)->m_sprite.GetBitmapIndex(),
		D2D1::RectF(region.l, region.t, region.r, region.b)
	);

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

	m_pGraphics->EndDraw();

}