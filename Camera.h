#pragma once

#include "DataTypes.h"


class Camera
{
public:
	Camera(game::Float2* pLocation);

	void Offset(game::Float2 amount, float deltaTime);
	void OffsetTo(game::Float2 destination, float deltaTime);

	void SetZoom(float amount);
	void DecreaseZoom(float factor);
	void IncreaseZoom(float factor);

	float GetZoom();

	game::Float2 ScreenLocToWorldLoc(float screenX, float screenY);
	game::Float2 WorldLocToScreenLoc(float worldX, float worldY);
	game::Rect WorldTransformToScreenRect(game::Float2 location, game::Float2 size);

	float SU_to_WU(float screenUnit);
	float WU_to_SU(float worldUnit);

	static game::Int2 m_screenResolution;

protected:
	game::Float2& m_origin;
	game::Float2 m_offset;
	float m_zoom;
	float m_speed;
};
