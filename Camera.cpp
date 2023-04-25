#include "Camera.h"


game::Int2 Camera::m_screenResolution = { 1280,720 };

Camera::Camera(game::Float2* pLocation)
	:
	m_zoom(64.f),
	m_speed(4.f),
	m_origin(*pLocation),
	m_offset{0,0}
{
}

void Camera::OffsetTo(game::Float2 destination, float deltaTime)
{
	game::Float2 distance{ destination - (m_origin + m_offset) };

	m_offset += distance * (m_speed * deltaTime / 1000);
}

void Camera::SetZoom(float amount)
{
	m_zoom = amount;
}
void Camera::DecreaseZoom(float factor)
{
	m_zoom -= m_zoom * factor;
}
void Camera::IncreaseZoom(float factor)
{
	m_zoom += m_zoom * factor;
}

float Camera::GetZoom()
{
	return m_zoom;
}

game::Float2 Camera::ScreenLocToWorldLoc(float screenX, float screenY)
{
	// Loc to Corn (SU)
	game::Float2 A{ -screenX, -screenY };
	// WU
	A = {
		PX_to_WU(A.x),
		-PX_to_WU(A.y)
	};

	// Corn to Cam (SU)
	game::Float2 B{
		m_screenResolution.x / 2,
		m_screenResolution.y / 2
	};
	// WU
	B = {
		PX_to_WU(B.x),
		-PX_to_WU(B.y)
	};

	// Cam to Cent (WU)
	game::Float2 C{ -(m_origin + m_offset) };

	// Loc to Cent
	game::Float2 D{ A + B + C };

	// Cent to Loc
	return -D;
}
game::Float2 Camera::WorldLocToScreenLoc(float worldX, float worldY)
{
	// Loc to Cent (WU)
	game::Float2 A{ -worldX, -worldY };
	// SU
	A = {
		WU_to_PX(A.x),
		-WU_to_PX(A.y)
	};

	// Cent to Cam (WU)
	game::Float2 B{ m_origin + m_offset };
	// SU
	B = {
		WU_to_PX(B.x),
		-WU_to_PX(B.y)
	};

	// Cam to Corn (SU)
	game::Float2 C{
		-m_screenResolution.x / 2,
		-m_screenResolution.y / 2
	};

	// Loc to Corn
	game::Float2 D{ A + B + C };

	// Corn to Loc
	return -D;
}
game::Rect Camera::WorldTransformToScreenRect(game::Float2 location, game::Float2 size)
{
	game::Float2 bottom_left{
		WorldLocToScreenLoc(
			location.x - size.x / 2,
			location.y - size.y / 2
		)
	};
	game::Float2 top_right{
		WorldLocToScreenLoc(
			location.x + size.x / 2,
			location.y + size.y / 2
		)
	};

	return {
		bottom_left.x,
		top_right.y,
		top_right.x,
		bottom_left.y
	};
}

float Camera::PX_to_WU(float screenUnit)
{
	return screenUnit / m_zoom;
}
float Camera::WU_to_PX(float worldUnit)
{
	return worldUnit * m_zoom;
}