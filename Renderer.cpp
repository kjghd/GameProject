#include "Renderer.h"

#include "Prefabs.h"

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
	for (const auto& tile : m_pScene->vTiles)
	{
		game::Rect rect{
			m_pCamera->WorldTransformToScreenRect(
				tile.m_location,
				tile.m_size
			)
		};

		m_pGraphics->DrawBitmap(
			D2D1::RectF(rect.l, rect.t, rect.r, rect.b),
			tile.m_texture
		);
	}

	// Block preview
	m_pGraphics->DrawBitmap(
		D2D1::RectF(0, 0, 40, 40),
		prefabList.Get(m_pScene->current_prefab)->m_texture
	);

	// Player
	game::Rect playerRect{
		m_pCamera->WorldTransformToScreenRect(
			m_pScene->player.m_location + m_pScene->player.m_spriteOffset,
			m_pScene->player.m_size
		)
	};

	m_pGraphics->DrawBitmap(
		D2D1::RectF(playerRect.l,playerRect.t, playerRect.r, playerRect.b),
		m_pScene->player.m_texture
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
	
	/*game::Float2 center{ m_pCamera->WorldLocToScreenLoc(0,0) };

	m_pGraphics->DrawBitmap(
		D2D1::RectF(center.x - 10, center.y - 10, center.x + 10, center.y + 10),
		T_Error
	);*/

	m_pGraphics->EndDraw();

}