#pragma once

#include "DataTypes.h"

class GameObject;

class Camera
{
public:
	Camera(GameObject* pOwner);

	void OffsetTo(game::float2 destination, float deltaTime);

	void SetZoom(float amount);
	void DecreaseZoom(float factor);
	void IncreaseZoom(float factor);

	float GetZoom();

	game::float2 ScreenLocToWorldLoc(float screenX, float screenY);
	game::float2 WorldLocToScreenLoc(float worldX, float worldY);
	game::rect WorldTransformToScreenRect(game::float2 location, game::float2 size);

	float PX_to_WU(float pixels);
	float WU_to_PX(float worldUnits);

	static game::float2 m_screenResolution;

	virtual std::string Serialise();

protected:
	GameObject* pOwner;
	game::float2 m_offset;
	float m_zoom; // Pixels per world unit.
	float m_speed;
};
