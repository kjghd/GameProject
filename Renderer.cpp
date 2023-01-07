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

	// Blocks
	std::sort(m_pScene->vpGameObjects.begin(), m_pScene->vpGameObjects.end(), GameObject::CompareRenderLayer);

	for (int i{ SL_COUNT }; i >= 0 ; --i )
		for (auto& pGameObject : m_pScene->vpGameObjects)
		{
			if (pGameObject->m_sprite.layer == i)
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
		}

	// Block preview
	m_pGraphics->DrawBitmap(
		D2D1::RectF(0, 0, 60, 60),
		prefabList.Get(m_pScene->current_prefab)->m_sprite.texture
	);

	// Player
	game::Rect playerRect{
		m_pCamera->WorldTransformToScreenRect(
			m_pScene->player.m_location + m_pScene->player.m_sprite.offset,
			m_pScene->player.m_sprite.size
		)
	};

	m_pGraphics->DrawBitmap(
		D2D1::RectF(playerRect.l,playerRect.t, playerRect.r, playerRect.b),
		m_pScene->player.m_sprite.texture
	);
	// Player Collision Debug
	game::Float2 playerLoc{
		m_pCamera->WorldLocToScreenLoc(
			m_pScene->player.m_location.x,
			m_pScene->player.m_location.y
		)
	};
	m_pGraphics->DebugCircle(
		{playerLoc.x, playerLoc.y},
		m_pCamera->WU_to_SU(m_pScene->player.m_collider.radius)
	);

	m_pGraphics->EndDraw();

}