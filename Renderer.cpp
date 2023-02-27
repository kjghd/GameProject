#include "Renderer.h"
#include "Prefabs.h"

#include <algorithm>

Renderer::Renderer()
	:
	m_pGraphics(nullptr),
	m_pScene(nullptr),
	m_pCamera(nullptr)
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
				// under
				if (pGameObject->m_sprite.layer > pLowestFound->m_sprite.layer)
					pLowestFound = pGameObject;

				// above
				else if (pGameObject->m_location.y > pLowestFound->m_location.y)
					pLowestFound = pGameObject;

				// row and left of
				else if (pGameObject->m_location.y == pLowestFound->m_location.y &&
					pGameObject->m_location.x < pLowestFound->m_location.x)
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
	for (auto& pGameObject : vpSorted)
	{
		game::Rect rect{
				m_pCamera->WorldTransformToScreenRect(
				pGameObject->m_location + pGameObject->m_sprite.offset,
				pGameObject->m_sprite.size
				)
		};

		m_pGraphics->DrawBitmap(
			D2D1::RectF(rect.l, rect.t, rect.r, rect.b),
			pGameObject->m_sprite.texture
		);
	}


	// Block preview
	m_pGraphics->DrawBitmap(
		D2D1::RectF(0, 0, 60, 60),
		prefabList.Get(m_pScene->current_prefab)->m_sprite.texture
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