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
	

	/* World Objects */

	// Sort for rendering.
	std::vector<WorldObject*> vpUnsorted{ m_pScene->vpGameObjects };
	std::vector<WorldObject*> vpSorted;
	while (!vpUnsorted.empty())
	{
		// Find lowest sprite.
		WorldObject* pLowestFound{ nullptr };
		for (auto& pGameObject : vpUnsorted)
		{
			if (!pLowestFound)
				pLowestFound = pGameObject;

			else if (pGameObject != pLowestFound)
			{
				if (WorldObject::CompareRenderOrder_Under(pGameObject, pLowestFound))
					pLowestFound = pGameObject;

				else if (WorldObject::CompareRenderOrder_Above(pGameObject, pLowestFound))
					pLowestFound = pGameObject;

				else if (WorldObject::CompareRenderOrder_RowAndLeftOf(pGameObject, pLowestFound))
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
					D2D1::RectF(region.l, region.t, region.r, region.b),
					1,
					pGameObject->m_sprite.CheckInvertedX(),
					pGameObject->m_sprite.CheckInvertedY()
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


	/* User Interface */

	if (m_pScene->state == SState_Pause)
	{
		game::Rect bg_source{ m_pScene->ui_background->GetSourceRect() };
		game::Rect bg_screen{ m_pScene->ui_background->GetScreenRect() };

		m_pGraphics->DrawBitmapRegion(
			{ bg_screen.l, bg_screen.t, bg_screen.r, bg_screen.b },
			m_pScene->ui_background->GetBitmapIndex(),
			{ bg_source.l, bg_source.t, bg_source.r, bg_source.b }
		);

		game::Rect resume_source{ m_pScene->button_resume->GetSourceRect() };
		game::Rect resume_screen{ m_pScene->button_resume->GetScreenRect() };

		m_pGraphics->DrawBitmapRegion(
			{ resume_screen.l, resume_screen.t, resume_screen.r, resume_screen.b },
			m_pScene->button_resume->GetBitmapIndex(),
			{resume_source.l, resume_source.t, resume_source.r, resume_source.b}
		);

		game::Rect mainMenu_source{ m_pScene->button_mainMenu->GetSourceRect() };
		game::Rect mainMenu_screen{ m_pScene->button_mainMenu->GetScreenRect() };

		m_pGraphics->DrawBitmapRegion(
			{ mainMenu_screen.l, mainMenu_screen.t, mainMenu_screen.r, mainMenu_screen.b },
			m_pScene->button_mainMenu->GetBitmapIndex(),
			{ mainMenu_source.l, mainMenu_source.t, mainMenu_source.r, mainMenu_source.b }
		);
	}
	else if (m_pScene->state == SState_Run)
	{
		// Block preview
		game::Rect region{ m_pScene->prefabs.GetWorldObject(m_pScene->current_prefab)->m_sprite.GetSourceRect() };

		m_pGraphics->DrawBitmapRegion(
			D2D1::RectF(0, 0, 60, 60),
			m_pScene->prefabs.GetWorldObject(m_pScene->current_prefab)->m_sprite.GetBitmapIndex(),
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
	}

	m_pGraphics->EndDraw();

}