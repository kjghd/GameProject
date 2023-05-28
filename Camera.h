#pragma once
#include "FileWritable.h"
#include "DataTypes.h"

class GameObject;

class Camera : public FileWritable
{
protected:
	//virtual void WriteData(std::ostream& os) override;

public:
	Camera(GameObject* pOwner);
	Camera(GameObject* pOwner, std::istream& is);

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

	virtual void WriteData(std::ostream& os) override;

protected:
	GameObject* pOwner;
	game::float2 m_offset;
	float m_zoom; // Pixels per world unit.
	float m_speed;
};
