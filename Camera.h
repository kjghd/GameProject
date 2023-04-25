#pragma once

#include "DataTypes.h"


class Camera
{
public:
	Camera(game::Float2* pLocation);

	void OffsetTo(game::Float2 destination, float deltaTime);

	void SetZoom(float amount);
	void DecreaseZoom(float factor);
	void IncreaseZoom(float factor);

	float GetZoom();

	game::Float2 ScreenLocToWorldLoc(float screenX, float screenY);
	game::Float2 WorldLocToScreenLoc(float worldX, float worldY);
	game::Rect WorldTransformToScreenRect(game::Float2 location, game::Float2 size);

	float PX_to_WU(float pixels);
	float WU_to_PX(float worldUnits);

	static game::Int2 m_screenResolution;

protected:
	game::Float2& m_origin;
	game::Float2 m_offset;
	float m_zoom; // Pixels per world unit.
	float m_speed;
};
